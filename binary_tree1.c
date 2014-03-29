#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define L 32

typedef int BOOL;

typedef struct treenode {
	int cnt;
	struct treenode *lc;
	struct treenode *rc;
} node;

void insertcontent(node *root, int a) {
	node *newnode, *ptr;
	newnode = (node*)malloc(sizeof(node));

	if(root->lc != NULL) {
		ptr = root->lc;
		while(TRUE) {
			if(a < ptr->cnt) {
				if(ptr->lc == NULL) {
					ptr->lc = newnode;
					ptr = ptr->lc;
					break;
				} else {
					ptr = ptr->lc;
				}
			} else {
				if(ptr->rc == NULL) {
					ptr->rc = newnode;
					ptr = ptr->rc;
					break;
				} else {
					ptr = ptr->rc;
				}
			}
		}
	} else {
		root->lc = newnode;
		ptr = newnode;
	}
	ptr->cnt = a;
	ptr->lc = NULL;
	ptr->rc = NULL;
	return;
}

void deletecontent(node* root, int a) {
	BOOL wasleft;
	node *child, *parent, *minchild, *minparent;

	//真の根は値を持たず常に一定で，その左の子が実質の根とする;
	parent = root;
	wasleft= TRUE;
	child = root->lc;

	while(child != NULL && child->cnt != a) {
		if(a < child->cnt) {
			if(child->lc == NULL) {
				child = NULL;
			} else {
				wasleft = TRUE;
				parent = child;
				child = child->lc;
			}
		} else {
			if (child->rc == NULL) {
				child = NULL;
			} else {
				wasleft = FALSE;
				parent = child;
				child = child->rc;
			}
		}
	}

	if(child == NULL) {
		printf("delete: %d was not found\n", a);
	} else {
		if(child->lc == NULL) {
			//左右共にそれ以上子がない場合;
			if(child->rc == NULL) {
				if(wasleft) {
					parent->lc = NULL;
				} else {
					parent->rc = NULL;
				}
				free(child);
			//右だけさらに子がある場合;
			} else {
				if(wasleft) {
					parent->lc = child->rc;
				} else {
					parent->rc = child->rc;
				}
				free(child);
			}
		} else {
			//左だけさらに子がある場合;
			if(child->rc == NULL) {
				if(wasleft) {
					parent->lc = child->lc;
				} else {
					parent->rc = child->lc;
				}
				free(child);
			//左右共にさらに子がある場合;
			//右の子孫から最小値をとってくる;
			} else {
				minparent = child;
				minchild = child->rc;

				if(minchild->lc == NULL) {
					minparent->rc = minchild->rc;
				} else {
					while(minchild->lc != NULL) {
						minparent = minchild;
						minchild = minchild->lc;
					}
					minparent->lc = minchild->rc;
				}
				child->cnt = minchild->cnt;
				free(minchild);
			}
		}
	}

	return;
}

void shcnt(node *root) {
	printf("%d",root->cnt);
	if(root->lc == NULL) {
		if(root->rc != NULL) {
			printf("-<*,"); shcnt(root->rc); printf(">");
		}
	} else {
		printf("-<"); shcnt(root->lc); printf(",");
		if(root->rc == NULL) {
			printf("*>");
		} else {
			shcnt(root->rc); printf(">");
		}
	}
	return;
}

void showcontent(node *root) {
	printf("content(s): ");
	if(root->lc == NULL) {
		printf("empty");
	} else {
		shcnt(root->lc);
	}
	printf("\n");
	return;
}

void freetree(node *root) {
	if(root->lc != NULL) { freetree(root->lc); }
	if(root->rc != NULL) { freetree(root->rc); }
	free(root);
	return;
}

void freeall(node *root) {
	node *ptr;
	if(root->lc != NULL) {
		freetree(root->lc);
	}
	return;
}

int main() {
	char cmdstr[L], *cmdmain, *cmdsub;
	int cmdint;
	node *bintreeroot;
	bintreeroot = (node*)malloc(sizeof(node));
	bintreeroot->lc = NULL;
	bintreeroot->rc = NULL;//なくてもよい;

	printf("BINARY TREE by gfn\n");
	printf("input \"insert (integer)\" to insert datum\n");
	printf("input \"delete (integer)\" to delete datum\n");
	printf("input \"show\" to show stored data\n");
	printf("input \"quit\" to end\n\n");
	printf("binary_tree>");
	while(strcmp(gets(cmdstr), "quit")) {
		cmdmain = strtok(cmdstr, " ");
		cmdsub = strtok(NULL, " ");
		cmdint = atoi(cmdsub);

		if(strcmp(cmdmain, "insert") == 0) {
			insertcontent(bintreeroot, cmdint);
		} else {
			if(strcmp(cmdmain, "delete") == 0) {
				deletecontent(bintreeroot, cmdint);
			} else {
				if(strcmp(cmdmain, "show") != 0) {
					printf("syntax error\n");
				}
			}
		}
		showcontent(bintreeroot);
		printf("binary_tree>");
	}
	freeall(bintreeroot);
	return EXIT_SUCCESS;
}
