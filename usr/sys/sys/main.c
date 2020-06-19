#include "../h/param.h"
#include "../h/systm.h"
#include "../h/dir.h"
#include "../h/user.h"
#include "../h/filsys.h"
#include "../h/mount.h"
#include "../h/map.h"
#include "../h/proc.h"
#include "../h/inode.h"
#include "../h/seg.h"
#include "../h/conf.h"
#include "../h/buf.h"

extern unsigned int _udot_base;

/* XXX: prototypes */
extern void startup(void);
extern unsigned int translate_va_to_pa(unsigned int a);
extern void clkstart(void);
extern void cinit(void);
extern int newproc(void);
extern void panic(char *s);                                     /* sys/prf.c */
extern void bcopy(caddr_t from, caddr_t to, int count);         /* sys/subr.c */
extern void brelse(struct buf *bp);                             /* dev/bio.c */
extern void expand(int newsize);                                /* sys/slp.c */
extern int estabur(unsigned int nt, unsigned int nd, unsigned int ns, int sep, int xrw);  /* sys/ureg.c */
extern int copyout(const unsigned int *src, unsigned int *dst, unsigned int sz);  /* <asm> */
extern void sched(void);                                        /* sys/slp.c */
extern struct buf * bread(dev_t dev, daddr_t blkno);            /* dev/bio.c */
extern struct inode * iget(dev_t dev, ino_t ino);               /* sys/iget.c */
extern struct buf * geteblk(void);

/* forward declarations */
static void binit(void);
static void iinit(void);
/* XXX: end prototypes */

/*
 * Initialization code.
 * Called from cold start routine as
 * soon as a stack and segmentation
 * have been established.
 * Functions:
 *	clear and free user core
 *	turn on clock
 *	hand craft 0th process
 *	call all initialization routines
 *	fork - process 0 to schedule
 *	     - process 1 execute bootstrap
 *
 * loop at low address in user mode -- /etc/init
 *	cannot be executed.
 */
void main(void)
{

	startup();
	/*
	 * set up system process
	 */

	proc[0].p_addr = btoc(translate_va_to_pa(_udot_base));
	proc[0].p_size = USIZE;  /* USIZE*click, so 1*4096 */
	proc[0].p_stat = SRUN;
	proc[0].p_flag |= SLOAD|SSYS;
	proc[0].p_nice = NZERO;
	u.u_procp = &proc[0];
	u.u_cmask = CMASK;

	/*
	 * Initialize devices and
	 * set up 'known' i-nodes
	 */

	clkstart();
	cinit();
	binit();
	iinit();
	rootdir = iget(rootdev, (ino_t)ROOTINO);
	rootdir->i_flag &= ~ILOCK;
	u.u_cdir = iget(rootdev, (ino_t)ROOTINO);
	u.u_cdir->i_flag &= ~ILOCK;
	u.u_rdir = NULL;

	/*
	 * make init process
	 * enter scheduling loop
	 * with system process
	 */

	if(newproc()) {
		expand(USIZE + (int)btoc(szicode));
		estabur((unsigned)0, btoc(szicode), (unsigned)0, 0, RO);
		copyout((unsigned int *)(caddr_t)icode, (unsigned int *)(caddr_t)0, szicode);
		/*
		 * Return goes to loc. 0 of user init
		 * code just copied out.
		 */
		return;
	}
	sched();
}

/*
 * iinit is called once (from main)
 * very early in initialization.
 * It reads the root's super block
 * and initializes the current date
 * from the last modified date.
 *
 * panic: iinit -- cannot read the super
 * block. Usually because of an IO error.
 */
static void iinit(void)
{
	struct buf *cp, *bp;
	struct filsys *fp;

	(*bdevsw[major(rootdev)].d_open)(rootdev, 1);
	bp = bread(rootdev, SUPERB);
	cp = geteblk();
	if(u.u_error)
		panic("iinit");
	bcopy(bp->b_un.b_addr, cp->b_un.b_addr, sizeof(struct filsys));
	brelse(bp);
	mount[0].m_bufp = cp;
	mount[0].m_dev = rootdev;
	fp = cp->b_un.b_filsys;
	fp->s_flock = 0;
	fp->s_ilock = 0;
	fp->s_ronly = 0;
	time = fp->s_time;
}

/*
 * This is the set of buffers proper, whose heads
 * were declared in buf.h.  There can exist buffer
 * headers not pointing here that are used purely
 * as arguments to the I/O routines to describe
 * I/O to be done-- e.g. swbuf for
 * swapping.
 */
char	buffers[NBUF][BSIZE+BSLOP];

/*
 * Initialize the buffer I/O system by freeing
 * all buffers and setting all device buffer lists to empty.
 */
static void binit(void)
{
	register struct buf *bp;
	register struct buf *dp;
	register int i;
	struct bdevsw *bdp;

	bfreelist.b_forw = bfreelist.b_back =
	    bfreelist.av_forw = bfreelist.av_back = &bfreelist;
	for (i=0; i<NBUF; i++) {
		bp = &buf[i];
		bp->b_dev = NODEV;
		bp->b_un.b_addr = buffers[i];
		bp->b_back = &bfreelist;
		bp->b_forw = bfreelist.b_forw;
		bfreelist.b_forw->b_back = bp;
		bfreelist.b_forw = bp;
		bp->b_flags = B_BUSY;
		brelse(bp);
	}
	for (bdp = bdevsw; bdp->d_open; bdp++) {
		dp = bdp->d_tab;
		if(dp) {
			dp->b_forw = dp;
			dp->b_back = dp;
		}
		nblkdev++;
	}
}
