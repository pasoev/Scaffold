#include "parselvl.h"
#include "ledge.h"
#define BUFSIZE 4096

static char stack[8*1024];

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

	yxml_ret_t r;
	yxml_t x[1];
	yxml_init(x, stack, sizeof(stack));
	/* */
	char *doc; /* The XML document as a zero-terminated string */
	for(; *doc; doc++) {
		yxml_ret_t r = yxml_parse(x, *doc);
		if(r < 0){
			perror(NULL);
			exit(1); /* Handle error */
		}
		/* Handle any tokens we are interested in */
		printf("%s\n", x);
		printf("%s\n", *doc);
	}

	
	r = yxml_eof(x);
	if(r < 0)
		exit(1); /* Handle error */
	else{
		/* No errors in the XML document */
	}

	return list_size(ledges);
}

int main(int argc, char *argv[]){
	if(argc != 2){
		exit(EXIT_FAILURE);
	}
	char *fname = argv[1];
	List *ledges;
	parseLedges(ledges, fname, addLedge);
	return 0;
}
