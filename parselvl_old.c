#include "parselvl.h"

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

int parseLedges(List *ledges, char *filename, void (*addLedge)(List *ledges, int x, int y, int w, int h, const char *texturefilename, SDL_Renderer *renderer)){
  xmlDoc         *document;
  xmlNode        *root, *node;
  if (strlen(filename) < 0) {
    fprintf(stderr, "Usage: filename.xml\n");
    return -1;
  }
  LIBXML_TEST_VERSION

  document = xmlReadFile(filename, NULL, 0);
  root = xmlDocGetRootElement(document);
  /* fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type); */
  makeElements(root, ledges, addLedge);

  /* Clean up */
  /* Free document. */
  xmlFreeDoc(document);

  /* Free globals */
  xmlCleanupParser();
  
  return list_size(ledges);
}


	  
