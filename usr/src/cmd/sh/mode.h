#
/*
 *	UNIX shell
 */


#define BYTESPERWORD	(sizeof(char *))

typedef char	CHAR;
typedef char	BOOL;
typedef int	UFD;
typedef int	INT;
typedef float	REAL;
typedef void	*ADDRESS;
typedef long int	L_INT;
typedef unsigned	POS;
typedef char	*STRING;
typedef char	MSG[];
typedef int	PIPE[];
typedef char	*STKPTR;
typedef char	*BYTPTR;

typedef struct stat	STATBUF;	/* defined in /usr/sys/stat.h */
typedef struct blk	*BLKPTR;
typedef struct fileblk	FILEBLK;
typedef struct filehdr	FILEHDR;
typedef struct fileblk	*FILE;
typedef struct trenod	*TREPTR;
typedef struct forknod	*FORKPTR;
typedef struct comnod	*COMPTR;
typedef struct swnod	*SWPTR;
typedef struct regnod	*REGPTR;
typedef struct parnod	*PARPTR;
typedef struct ifnod	*IFPTR;
typedef struct whnod	*WHPTR;
typedef struct fornod	*FORPTR;
typedef struct lstnod	*LSTPTR;
typedef struct argnod	*ARGPTR;
typedef struct dolnod	*DOLPTR;
typedef struct ionod	*IOPTR;
typedef struct namnod	NAMNOD;
typedef struct namnod	*NAMPTR;
typedef struct sysnod	SYSNOD;
typedef struct sysnod	*SYSPTR;
#define NIL	((void*)0)


/* the following nonsense is required
 * because casts turn an Lvalue
 * into an Rvalue so two cheats
 * are necessary, one for each context.
 */
union { int _cheat;};
#define Lcheat(a)	((a)._cheat)
#define Rcheat(a)	((int)(a))


/* address puns for storage allocation */
typedef union {
	FORKPTR	_forkptr;
	COMPTR	_comptr;
	PARPTR	_parptr;
	IFPTR	_ifptr;
	WHPTR	_whptr;
	FORPTR	_forptr;
	LSTPTR	_lstptr;
	BLKPTR	_blkptr;
	NAMPTR	_namptr;
	BYTPTR	_bytptr;
	}	address;


/* heap storage */
struct blk {
	BLKPTR	word;
};

#define	BUFSIZ	64
struct fileblk {
	UFD	fdes;
	POS	flin;
	BOOL	feof;
	CHAR	fsiz;
	STRING	fnxt;
	STRING	fend;
	STRING	*feval;
	FILE	fstak;
	CHAR	fbuf[BUFSIZ];
};

/* for files not used with file descriptors */
struct filehdr {
	UFD	fdes;
	POS	flin;
	BOOL	feof;
	CHAR	fsiz;
	STRING	fnxt;
	STRING	fend;
	STRING	*feval;
	FILE	fstak;
	CHAR	_fbuf[1];
};

struct sysnod {
	STRING	sysnam;
	INT	sysval;
};

/* this node is a proforma for those that follow */
struct trenod {
	INT	tretyp;
	IOPTR	treio;
};

/* dummy for access only */
struct argnod {
	ARGPTR	argnxt;
	CHAR	argval[1];
};

struct dolnod {
	DOLPTR	dolnxt;
	INT	doluse;
	CHAR	dolarg[1];
};

struct forknod {
	INT	forktyp;
	IOPTR	forkio;
	TREPTR	forktre;
};

struct comnod {
	INT	comtyp;
	IOPTR	comio;
	ARGPTR	comarg;
	ARGPTR	comset;
};

struct ifnod {
	INT	iftyp;
	TREPTR	iftre;
	TREPTR	thtre;
	TREPTR	eltre;
};

struct whnod {
	INT	whtyp;
	TREPTR	whtre;
	TREPTR	dotre;
};

struct fornod {
	INT	fortyp;
	TREPTR	fortre;
	STRING	fornam;
	COMPTR	forlst;
};

struct swnod {
	INT	swtyp;
	STRING	swarg;
	REGPTR	swlst;
};

struct regnod {
	ARGPTR	regptr;
	TREPTR	regcom;
	REGPTR	regnxt;
};

struct parnod {
	INT	partyp;
	TREPTR	partre;
};

struct lstnod {
	INT	lsttyp;
	TREPTR	lstlef;
	TREPTR	lstrit;
};

struct ionod {
	INT	iofile;
	STRING	ioname;
	IOPTR	ionxt;
	IOPTR	iolst;
};

#define	FORKTYPE	(sizeof(struct forknod))
#define	COMTYPE		(sizeof(struct comnod))
#define	IFTYPE		(sizeof(struct ifnod))
#define	WHTYPE		(sizeof(struct whnod))
#define	FORTYPE		(sizeof(struct fornod))
#define	SWTYPE		(sizeof(struct swnod))
#define	REGTYPE		(sizeof(struct regnod))
#define	PARTYPE		(sizeof(struct parnod))
#define	LSTTYPE		(sizeof(struct lstnod))
#define	IOTYPE		(sizeof(struct ionod))
