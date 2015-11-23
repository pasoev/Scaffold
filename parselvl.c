#include "parselvl.h"
#include "ledge.h"
#define BUFSIZE 4096

static char stack[8*1024];
static int verbose = 0;

static void y_printchar(char c) {
	if(c == '\x7F' || (c >= 0 && c < 0x20))
		printf("\\x%02x", c);
	else
		printf("%c", c);
}


static void y_printstring(const char *str) {
	while(*str) {
		y_printchar(*str);
		str++;
	}
}


static void y_printtoken(yxml_t *x, const char *str) {
	puts("");
	if(verbose)
		printf("t%03"PRIu64" l%03"PRIu32" b%03"PRIu64": ",
		       x->total, x->line, x->byte);
	/* printf("%s", str);*/
}

static void y_printres(yxml_t *x, yxml_ret_t r) {
	static int indata;
	int nextdata = 0;

	int pos_x, pos_y, w, h;
	/*
	  if(!xmlStrcmp(curNode->name, (const xmlChar *)"object")){
	  xChar = xmlGetProp(curNode, "x");
	  yChar = xmlGetProp(curNode, "y");
	  wChar = xmlGetProp(curNode, "width");
	  hChar = xmlGetProp(curNode, "height");
	  textureFilename = xmlGetProp(curNode, "textureFile");
				
	  printf("Making ledge: %d %d %d %d %s\n", x, y, w, h, textureFilename);
	  addLedge(ledges, x, y, w, h, textureFilename, NULL);
	*/

	
	switch(r) {
	case YXML_OK:
		if(verbose) {
			y_printtoken(x, "ok");
			nextdata = 0;
		} else
			nextdata = indata;
		break;
	case YXML_ELEMSTART:
		y_printtoken(x, "elemstart ");
		y_printstring(x->elem);
		
		if(yxml_symlen(x, x->elem) != strlen(x->elem))
			y_printtoken(x, "assertfail: elem lengths don't match");
		if(r & YXML_CONTENT)
			y_printtoken(x, "content");
		break;
	case YXML_ELEMEND:
		printf("\n-----\nx = %d, y = %d, w = %d, h = %d\n-----\n", pos_x, pos_y, w, h);
		y_printtoken(x, "elemend");
		break;
	case YXML_ATTRSTART:
		y_printtoken(x, "attrstart ");
		y_printstring(x->attr);
		if(yxml_symlen(x, x->attr) != strlen(x->attr))
			y_printtoken(x, "assertfail: attr lengths don't match");
		break;
	case YXML_ATTREND:
		y_printtoken(x, "attrend");
		break;
	case YXML_PICONTENT:
	case YXML_CONTENT:
	case YXML_ATTRVAL:
		if(!indata){
			y_printtoken(x, r == YXML_CONTENT ? "content " : r == YXML_PICONTENT ? "picontent " : "attrval ");
		}
		if(!strcmp("x", x->attr)){
			pos_x = atoi(x->data);
		}else if(!strcmp("y", x->attr)){
			pos_y = atoi(x->data);
		}else if(!strcmp("width", x->attr)){
			w = atoi(x->data);
		}else if(!strcmp("height", x->attr)){
			h = atoi(x->data);
		}
		/* y_printstring(x->data); */
		nextdata = 1;
		break;
	case YXML_PISTART:
		y_printtoken(x, "pistart ");
		y_printstring(x->pi);
		if(yxml_symlen(x, x->pi) != strlen(x->pi))
			y_printtoken(x, "assertfail: pi lengths don't match");
		break;
	case YXML_PIEND:
		y_printtoken(x, "piend");
		break;
	default:
		y_printtoken(x, "error\n");
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

/*
  void makeElements(xmlNode *node, List *ledges, void (*addLedge)(List *ledges, int x, int y, int w, int h, const char *texturefilename, SDL_Renderer *renderer)){
  xmlNode *curNode = NULL;

  for(curNode = node; curNode != NULL; curNode = curNode->next){
  if(curNode->type == XML_ELEMENT_NODE){
  printf("%s\n", curNode->name);
  xmlChar *xChar, *yChar, *wChar, *hChar, *textureFilename;
  int x, y, w, h;
  if(!xmlStrcmp(curNode->name, (const xmlChar *)"object")){
  xChar = xmlGetProp(curNode, "x");
  yChar = xmlGetProp(curNode, "y");
  wChar = xmlGetProp(curNode, "width");
  hChar = xmlGetProp(curNode, "height");
  textureFilename = xmlGetProp(curNode, "textureFile");
  x = atoi(xChar);
  y = atoi(yChar);
  w = atoi(wChar);
  h = atoi(hChar);
  printf("Making ledge: %d %d %d %d %s\n", x, y, w, h, textureFilename);
  addLedge(ledges, x, y, w, h, textureFilename, NULL);
  xmlFree(xChar);
  xmlFree(yChar);
  xmlFree(wChar);
  xmlFree(hChar);
  xmlFree(textureFilename);
  }
  }
  makeElements(curNode->children, ledges, addLedge);
  }
  }


*/
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
	fp = fopen (  filename , "r" );
	if( !fp ) perror(filename),exit(1);

	fseek( fp , 0L , SEEK_END); 
	lSize = ftell( fp );
	rewind( fp ); 
	
	
	while((c = getc(fp)) != EOF) {
		r = yxml_parse(x, c);
		y_printres(x, r);
	}

	fclose(fp);
	y_printtoken(x, yxml_eof(x) < 0 ? "error\n" : "ok\n");

	return list_size(ledges);
}

int main(int argc, char *argv[]){
	if(argc != 2){
		exit(EXIT_FAILURE);
	}
	char *fname = argv[1];
	verbose = argc > 2;
	
	List *ledges;	
	parseLedges(ledges, fname, addLedge);
	return 0;
}
