
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATHS 1000
#define MAX_PATH_LEN 1000
#define MAX_FOLDER 1000

struct Node {
  char name[100];
  int isFile;            // 1: File, 0: Folder
  char path[1000];      // Đường dẫn
  struct Node *sibling;  
  struct Node *child;    
};
typedef struct Node Node;

Node* makeNode(const char* name, int isFile) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  strcpy(newNode->name, name);
  strcpy(newNode->path, "");
  newNode->isFile = isFile;
  newNode->sibling = NULL;
  newNode->child = NULL;
  return newNode;
}

// Thêm node con vào thư mục cha
void addNode(Node** parent, const char* name, int isFile) {
  Node* newNode = makeNode(name, isFile);
  if(strcmp((*parent)->path, "") == 0){
  	strcpy(newNode->path, (*parent)->name);
  	strcat(newNode->path, "/");
	}
  else{
  	strcpy(newNode->path, (*parent)->path);
  	strcat(newNode->path, "/");
	}
	strcat(newNode->path, name);
  if ((*parent)->child == NULL){
    (*parent)->child = newNode;
  }
	else{
    Node* current = (*parent)->child;
    while (current->sibling != NULL) {
      current = current->sibling;
    }
    current->sibling = newNode;
  }
}

Node* findDfs(Node* current, const char* Name) {
  if (current == NULL) return NULL;
  if (strcmp(current->name, Name) == 0) {
    return current;
  }
  // Tìm trong các node con
  Node* foundInChild = findDfs(current->child, Name);
  if (foundInChild != NULL) return foundInChild;

  return findDfs(current->sibling, Name);
}

void freeTree(Node* node) {
  if (node == NULL) return;
  freeTree(node->child);    // Giải phóng cây con
  freeTree(node->sibling);  // Giải phóng các node anh em
  free(node);
}

void xoaAE(Node** head, const char* name){
	Node* del = findDfs(*head, name);
	if(del == NULL) return;
	if(del == *head){
		*head = del->sibling;
		freeTree(del->child);
		free(del);
	}
	else{
		Node* tmp = *head;
		while(tmp->sibling != del){
			tmp = tmp->sibling;
		}
		tmp->sibling = del->sibling;
		freeTree(del->child);
		free(del);
	}
}

void updatePath(Node* node, const char* newParentPath) {
  if (node == NULL) return;

  snprintf(node->path, MAX_PATH_LEN, "%s/%s", newParentPath, node->name);

  updatePath(node->child, node->path);

  updatePath(node->sibling, newParentPath);
}

// Lưu các đường dẫn tìm được
Node* arr[MAX_FOLDER];
int folderCount = 0;
void findAllPaths(Node* current, const char* name) {
  if (current == NULL) return;
	if(strcmp(current->name, name) == 0) arr[folderCount++] = current;
  // Tiếp tục duyệt qua con và anh em
  findAllPaths(current->child, name);
  findAllPaths(current->sibling, name); // Anh em giữ nguyên đường dẫn cha
}

void printPaths(){
  if (folderCount == 0){
    printf("Khong tim thay thu muc nao khop.\n");
  } 
	else{
    printf("Cac duong dan den thu muc khop:\n");
    for (int i = 0; i < folderCount; i++) {
      printf("%d: %s\n", i + 1, arr[i]->path);
    }
  }
}

void in(Node* roots, int cnt){       // hiển thị toàn bộ thư mục
	while(roots != NULL){
		for(int i = 0; i < cnt; i++) printf("   ");
		printf("%s%s\n", roots->name, roots->isFile? " (File)" : " (Folder)");
		in(roots->child, cnt + 1);
		roots = roots->sibling;
	}
}

void in2(Node* roots, int cnt){      // hiển thị 1 thư mục tổng quát nhất định
	printf("%s%s\n", roots->name, roots->isFile? " (File)" : " (Folder)");
	in(roots->child, cnt + 1);
}

void inCon(Node* roots, int cnt){    // Chỉ in các con bậc 1 của 1 nốt cha bất kì
	printf("Ten thu muc cha: %s\n", roots->name);
	Node* con = roots->child;
	while(con != NULL){
	  printf("%s%s\n", con->name, con->isFile? " (File)" : " (Folder)");
		con = con->sibling;
	}
}

int isFile ; 
Node* menu(Node** head){
	while(1){
		system("cls");
		printf("+-----------------------------------------------------------+\n");
		printf("|%s%-40s|\n", "Duong dan thu muc: ", (*head)->path              );
		printf("+-----------------------------------------------------------+\n");
		printf("|[1]. Sua ten thu muc                                       |\n");
		printf("|[2]. Them thuc muc con/tep vao thu muc hien tai            |\n");
		printf("|[3]. Xoa thu muc con/tep cua thu muc hien tai              |\n");
		printf("|[4]. Tim kiem thu muc con/tep trong thu muc hien tai       |\n");
		printf("|[5]. Hien thi cac thu muc con/tep                          |\n");
		printf("|[6]. Hien thi cay thu muc                                  |\n");
		printf("|[0]. Thoat                                                 |\n");
		printf("+-----------------------------------------------------------+\n");
		printf("Nhap lua chon: ");
		int lc2; scanf("%d", &lc2);
		if(lc2 == 0) break;
		else if(lc2 == 1){
			char newName[100];
			getchar();
			printf("Nhap ten moi muon sua: ");
			gets(newName);
			Node* check = findDfs((*head), newName);
			if(check != NULL){
				printf("Da ton tai thu muc/tep co ten: %s\n", newName);
				printf("Nhap phim bat ky de tiep tuc...");
				getchar();
				continue;
			}
			strcpy((*head)->name, newName);
			char arr[1000][1000];
			char* token = strtok((*head)->path, "/");
			int dem = 0;
			while(token != NULL){
				strcpy(arr[dem++], token);
			token = strtok(NULL, "/");
			}
			strcpy((*head)->path, "");
			for(int i = 0; i < dem - 1; i++){
				strcat((*head)->path, arr[i]);
				strcat((*head)->path, "/");
			}
			strcat((*head)->path, newName);
			updatePath((*head)->child, (*head)->path);
		}
		else if(lc2 == 2){
			getchar();
			char nameChild[100];
			printf("Nhap ten thu muc con/tep muon them vao: ");
			gets(nameChild);
			printf("Nhap loai (0: Thu muc, 1: Tep): ");
      scanf("%d", &isFile);
      getchar();
      if((*head)->isFile == 1){
      	printf("Khong the them vao tep \n");
      		printf("Nhap phim bat ky de tiep tuc...");
				getchar();
			}else{
			Node* check = findDfs((*head)->child, nameChild);
			if(check != NULL){
				printf("Da ton tai ten thu muc\n");
				printf("Nhap phim bat ky de tiep tuc...");
				getchar();
			}
			else{
				addNode(head, nameChild, isFile);
				printf("Them thanh cong\n");
				printf("Nhap phim bat ky de tiep tuc...");
				getchar();
			}
		}
	}
		else if(lc2 == 3){
			getchar();
			char delName[100];
			printf("Nhap ten thu muc/tep can xoa: ");	gets(delName);
			Node* del = findDfs((*head)->child, delName);
			if(del == NULL){
				printf("Khong ton tai thu muc\n");
				printf("Nhap phim bat ky de tiep tuc...");
				getchar();
			}
			else{
				xoaAE(&((*head)->child), delName);
				printf("Da xoa thanh cong\n");
				printf("Nhap phim bat ky de tiep tuc...");
				getchar();
			}
		}
		else if(lc2 == 4){
			getchar();
			char findName[100];
			printf("Nhap ten thu muc/tep can tim kiem: ");
			gets(findName);
			Node* tmpChild = findDfs((*head)->child, findName);
			if(tmpChild == NULL){
				printf("Khong ton tai thu muc\n");
				printf("Nhap phim bat ky de tiep tuc...");
				getchar();
				continue;
			}
			else{
				return tmpChild;
			}
		}
		else if(lc2 == 5){
			inCon((*head),0);
			printf("Nhap phim bat ky de tiep tuc...");
			getchar();
			getchar();
		}
		
		else if(lc2 == 6){
			in2(*head, 0);
			printf("Nhan phim bat ky de tiep tuc...");
			getchar();
			getchar();
		}
	}
	return NULL;
}

int main(){
	Node* thisPc = makeNode("This PC", 0); 
  int rootCount = 0;
  int choice;
  char parentName[100], name[100];
  int isFile;
	addNode(&thisPc, "nhat", 0);
	addNode(&thisPc, "khai", 0);
	addNode(&thisPc, "khoi", 0);
	
	addNode(&thisPc->child, "nhat1", 1);
  addNode(&thisPc->child, "nhat2", 1);
  addNode(&thisPc->child, "nhat3", 1);
  
  addNode(&thisPc->child->sibling, "khai1", 1);
  addNode(&thisPc->child->sibling, "khai2", 1);
  addNode(&thisPc->child->sibling, "khai3", 1);
  
  addNode(&thisPc->child->sibling->sibling, "khoi1", 1);
  addNode(&thisPc->child->sibling->sibling, "khoi2", 1);
  addNode(&thisPc->child->sibling->sibling, "nhat", 1);
  
  
  while(1){
  	system("cls");
  	printf("+------------------------------------------------+\n");
    printf("|                     This PC                    |\n");
    printf("+------------------------------------------------+\n");
    printf("|[1]. Tim kiem thu muc/tep     	                 |\n");
    printf("|[2]. Hien thi thu muc                           |\n");
    printf("|[3]. Tim kiem tat ca duong dan cua thu muc/tep  |\n");
    printf("|[4]. Hien thi cay thu muc                       |\n");
    printf("|[5]. Them thu muc/tep                           |\n");
    printf("|[6]. Xoa thu muc/tep                            |\n");
    printf("|[0]. Thoat                                      |\n");
    printf("+------------------------------------------------+\n");
    printf("Nhap lua chon: ");
    scanf("%d", &choice);
    
    if(choice == 0) break;
    
    else if(choice == 1){
    	getchar();
    	char findName[100];
      printf("Nhap ten thu muc can tim kiem: ");
      gets(findName);
      Node* tmp = findDfs(thisPc->child, findName);
      if(tmp == NULL){
      	printf("Khong ton tai thu muc\n");
      	printf("Nhap phim bat ky de tiep tuc...");
      	getchar();
			}
			else{
				Node* tmpCon = menu(&tmp);
				while(tmpCon != NULL){
					tmpCon = menu(&tmpCon);
				}
			}
		}
		
		else if(choice == 2){
			inCon(thisPc,0);
			printf("Nhap phim bat ky de tiep tuc...");
      getchar();
      getchar();
		}
		
		else if(choice == 3) {
    	folderCount = 0; // Đặt lại bộ đếm trước mỗi lần tìm kiếm
    	getchar();
    	char findName[100];
    	printf("Nhap ten thu muc muon tim kiem: ");
    	gets(findName);
    	printf("\n[DEBUG] Bat dau tim kiem tat ca duong dan ...\n");
    	findAllPaths(thisPc, findName); // Tìm tất cả đường dẫn
    	printf("[DEBUG] Hoan thanh tim kiem.\n");
    	printPaths(); // Hiển thị kết quả
   	 	if (folderCount == 0) {
        printf("Nhap phim bat ki de tiep tuc...");
        getchar();
        continue;
    }
    // Nếu có kết quả, cho phép người dùng chọn truy cập
   	 printf("Nhap lua chon de chon thu muc muon truy cap (1-%d): ", folderCount);
     int lc;
     scanf("%d", &lc);
     while (lc > folderCount || lc <= 0) {
       printf("Lua chon khong hop le. Vui long nhap lai... ");
       scanf("%d", &lc);
    }
     Node* tmp = menu(&arr[lc - 1]);
     while (tmp != NULL) {
        tmp = menu(&tmp);
    }
}
		
		else if(choice == 4){
    	in(thisPc, 0);
    	printf("Nhap phim bat ky de tiep tuc...");
      getchar();
      getchar();	
		}
		else if(choice == 5){
			getchar();
			char nameChild[100];
			printf("Nhap ten thu muc con/tep muon them vao: ");
			gets(nameChild);
			printf("Nhap loai (0: Thu muc, 1: Tep): ");
      scanf("%d", &isFile);
      getchar();
			Node* check = findDfs((thisPc)->child, nameChild);
			if(check != NULL){
				printf("Da ton tai ten thu muc\n");
				printf("Nhap phim bat ky de tiep tuc...");
				getchar();
			}
			else{
				addNode(&thisPc, nameChild, isFile);
				printf("Them thanh cong\n");
				printf("Nhap phim bat ky de tiep tuc...");
				getchar();
			}
		}
		else if(choice == 6){
			getchar();                                                                                                                                                              
			char delName[100];
			printf("Nhap ten thu muc can xoa: ");	gets(delName);
			Node* del = findDfs(thisPc->child, delName);
			if(del == NULL){
				printf("Khong ton tai thu muc\n");
				printf("Nhap phim bat ky de tiep tuc...");
				getchar();
			}
			else{
				xoaAE(&(thisPc->child), delName);
				printf("Da xoa thanh cong\n");
				printf("Nhap phim bat ky de tiep tuc...");
				getchar();
			}
		}
	}
}
