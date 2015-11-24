#include "parselvl.h"
#include "ledge.h"
#define BUFSIZE 4096

static char stack[8*1024];
static int verbose = 0;

char pos_xChr[20] = "", pos_yChr[20] = "", wChr[20] = "", hChr[20] = "";
int pos_x = 0, pos_y = 0, w = 0, h = 0;

static void y_printres(yxml_t *x, yxml_ret_t r) {
	static int indata;
	int nextdata = 0;

	/* declare the string for each attribute here. Concatenate on attrval. */


	/*
	  addLedge(ledges, x, y, w, h, textureFilename, NULL);
	*/

	
	switch(r) {
	case YXML_OK:
		if(verbose) {
			/* y_printtoken(x, "ok"); */
			nextdata = 0;
		} else
			nextdata = indata;
		break;
	case YXML_ELEMSTART:
		/* y_printtoken(x, "elemstart "); */
		/* y_printstring(x->elem); */
		
		if(yxml_symlen(x, x->elem) != strlen(x->elem)){
			/* y_printtoken(x, "assertfail: elem lengths don't match"); */
		}
		if(r & YXML_CONTENT){
			/* y_printtoken(x, "content");*/
		}
		
		break;
	case YXML_ELEMEND:
		/* printf("\n-----\nx = %d, y = %d, w = %d, h = %d\n-----\n", pos_x, pos_y, w, h); */
		/* y_printtoken(x, "elemend"); */
		printf("\n-----\nx = %d, y = %d, w = %d, h = %d\n-----\n", pos_x, pos_y, w, h);
		break;
	case YXML_ATTRSTART:
		/* y_printtoken(x, "attrstart "); */
		/* y_printstring(x->attr); */
		/* 
		if(yxml_symlen(x, x->attr) != strlen(x->attr))
			y_printtoken(x, "assertfail: attr lengths don't match");
		*/
		break;
	case YXML_ATTREND:
		/*
		y_printtoken(x, "attrend");
		*/
		if(!strcmp("x", x->attr)){
			pos_x = atoi(pos_xChr);
			memset(pos_xChr, 0, sizeof(pos_xChr));
		}else if(!strcmp("y", x->attr)){
			pos_y = atoi(pos_yChr);
			memset(pos_yChr, 0, sizeof(pos_yChr));
		}else if(!strcmp("width", x->attr)){
			w = atoi(wChr);
			memset(wChr, 0, sizeof(wChr));
		}else if(!strcmp("height", x->attr)){
			h = atoi(hChr);
			memset(hChr, 0, sizeof(hChr));
		}
		break;
	case YXML_PICONTENT:
	case YXML_CONTENT:
	case YXML_ATTRVAL:
		/*
		if(!indata){
			y_printtoken(x, r == YXML_CONTENT ? "content " : r == YXML_PICONTENT ? "picontent " : "attrval ");
		}
		*/
		if(!strcmp("x", x->attr)){
			strcat(pos_xChr, x->data);
		}else if(!strcmp("y", x->attr)){
			strcat(pos_yChr, x->data);
		}else if(!strcmp("width", x->attr)){
			strcat(wChr, x->data);
		}else if(!strcmp("height", x->attr)){
			strcat(hChr, x->data);
		}
		/* y_printstring(x->data); */
		nextdata = 1;
		break;
	case YXML_PISTART:
		/* y_printtoken(x, "pistart "); */
		/* y_printstring(x->pi); */
		if(yxml_symlen(x, x->pi) != strlen(x->pi)){
			/* y_printtoken(x, "assertfail: pi lengths don't match"); */
		}
		break;
	case YXML_PIEND:
		/*
		y_printtoken(x, "piend");
		*/
		break;
	default:
		/*
		y_printtoken(x, "error\n");
		*/
		exit(0);
	}
	indata = nextdata;
}

struct Ledge{
	int x;
	int y;
	int w;
	int h;
	SDL_Texture *texture;
};

void addLedge(List *ledges, int x, int y, int w, int h, const char *texturefilename, SDL_Renderer *renderer){
	struct Ledge *ledge = malloc(sizeof (struct Ledge));
	ledge->x = x;
	ledge->y = y;
	ledge->w = w;
	ledge->h = h;
	ledge->texture = NULL;
	list_ins_next(ledges, list_tail(ledges), ledge);
}

int parseLedges(List *ledges, char *filename,
		void (*addLedge)(List *ledges,
				 int x, int y, int w, int h,
				 const char *texturefilename,
				 SDL_Renderer *renderer)){
	int c;
	yxml_ret_t r;
	yxml_t x[1];
	yxml_init(x, stack, sizeof(stack));

	

	FILE *fp;
	long lSize;
	fp = fopen (  filename , "rb" );
	if( !fp ) perror(filename),exit(1);

	fseek( fp , 0L , SEEK_END); 
	lSize = ftell( fp );
	rewind( fp ); 
	
	
	while((c = getc(fp)) != EOF) {
		r = yxml_parse(x, c);
		y_printres(x, r);
	}

	fclose(fp);
	/* y_printtoken(x, yxml_eof(x) < 0 ? "error\n" : "ok\n"); */

	return list_size(ledges);
}

