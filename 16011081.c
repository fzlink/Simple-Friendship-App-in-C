#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
	int IDno; // KIMLIK NO
	char adSoyad[40]; // ISIM SOYISIM
	int arkadaslar[15]; // ARKADAS DIZISI
	struct node *right; // SAG DUGUM POINTERI
	struct node *left; // SOL DUGUM POINTERI
}NODE;

NODE *root;// GLOBAL OLARAK BİR ROOT TANIMLANDI.

//333333333333333333333333333333
NODE *contains(int id,int wprint){ // WPRINT DUGUMLERDE GEZERKEN YAZDIRIP YAZDIRMAYACAGIMIZI BELIRLIYOR
    NODE *node; //  AGACTA GEZMEK ICIN KULLANILAN POINTER
    node = root;
    if(wprint && node!= NULL)
        printf("%d\n",node->IDno);
    while((node!=NULL) && (id != node->IDno)){
        if(id<node->IDno)
            node=node->left;
        else
            node=node->right;
        if(wprint && node!= NULL)
            printf("%d\n",node->IDno);
    }
    return node;
}
//BBBBBBBBBBBBBBBBBBBBBBBBBBBBB


//111111111111111111111111111111
void insertNewUser(NODE *newNode){ //newNode readPerson fonksiyonundan gelen dugumdur.
	NODE *node,*prev; // *node agacta gezinmek icindir. *prev ise bir onceki dugumu tutmak icindir.
	node = root;
	int stop=0,id;  //  id kolaylık olması acisindan tutulmustur icerisine newNode->IDno atilir. stop degiskeni while in durmasi icindir.
	id=newNode->IDno;
	if(root==NULL){
		root = newNode;
	}
	else{
		while(!stop){
			if(id<node->IDno){
				prev = node;
				node = node->left;
				if(node==NULL){
					prev->left = newNode;
					stop=1;
				}
			}
			else{
				prev = node;
				node = node->right;
				if(node==NULL){
					prev->right = newNode;
					stop=1;
				}
			}
		}
	}
}

NODE * readPerson(char buffer[],int id){ // buffer, fgets ile dosyadan alinan satiri tutar. id, iceride dosyada boyle bir id olup olmadigini anlamak icin lazimdir.
	char tmp[5];  // Dosyadan okunan id lerin karakter karakter okunmasi icin tutuldu. Atoi fonksiyonuyla integera cevrilir.
	int i,j,k; // Dongu degiskenleri
	i=0;
	k=0;
	memset(tmp,0,sizeof(tmp));
	if(buffer[i] == '\n')
        return NULL;
	while((buffer[i] < '0' || buffer[i] > '9') && buffer[i] != '\n') //Verilen input.txt dosyasinin ilk satirinda gozukmeyen karakterler bulunnmakta, bu gibi durumlara karsi sayi gelene kadar ilerlemesi icin kontrol koyulmustur
        i++;
    j=0;
	while(buffer[i] != ','){
		tmp[j] = buffer[i];
		i++;
		j++;
	}
	if(id != atoi(tmp)){
		return NULL;
	}
	NODE *newNode;
	newNode = malloc(sizeof(NODE));
	newNode->left=newNode->right=NULL;
	memset(tmp,0,sizeof(tmp));
	i=j=0;
		while(i<(strlen(buffer)-1)){
            while(buffer[i] < '0' || buffer[i] > '9')
                i++;
			while(buffer[i] != ','){
				tmp[j++] = buffer[i++];
			}
			i++;
			j=0;
			newNode->IDno = atoi(tmp);
			while(buffer[i] != ',' && i<(strlen(buffer)-1)){
				newNode->adSoyad[j++] = buffer[i++];
			}
			newNode->adSoyad[j] = '\0';
			if(buffer[i] == ','){
				i++;
				j=k=0;
				while(i<(strlen(buffer)-1)){
					memset(tmp,0,sizeof(tmp));
					while(buffer[i] != '-' && buffer[i] != '\0' && buffer[i] != '\n'){
						tmp[j++] = buffer[i++];
					}
					j=0;
					i++;
					newNode->arkadaslar[k++] = atoi(tmp);
				}
			}
			newNode->arkadaslar[k] = -1;
		}
		return newNode;
}
//BBBBBBBBBBBBBBBBBBBBBBBBBBB

//222222222222222222222222222
void copyNode(NODE *to,NODE*from){ // *to içerisine *from dugumundeki veriler kopyalanır.
    int i=0; // Dongu degiskeni
    to->IDno=from->IDno;
    strcpy(to->adSoyad,from->adSoyad);
    while(from->arkadaslar[i] != -1){
        to->arkadaslar[i] = from->arkadaslar[i];
        i++;
    }
    to->arkadaslar[i] = -1;
}

NODE * findMinimum(NODE *tmp){ // *tmp Gezmeye baslanacak dugumu gosterir.
    NODE *current; // Agacta gezme icin kullanılır.
    current = tmp->right;
    while(current->left!=NULL)
        current = current->left;
    return current;
}

void deleteUser(int id){ // silinecek kisinin id sini alir.
    NODE *current,*prev,*min; // *current: Agacta gezinmek icin, *prev: Bir onceki dugumu tutmak icin, *min: Minimum dugumu tutmak icin
    int leftIsEmpty; // Dugumun solunun bos olup olmadigini kontrol amacli
    if(root->IDno==id){
        if(root->left==NULL && root->right==NULL){
            NODE *tmp = root;
            root = NULL;
            free(tmp);
            return;
        }
        else if(root->left==NULL || root->right==NULL){
            if(root->left==NULL)
                root = root->right;
            else
                root = root->left;
        }
        else{
            min = findMinimum(root);
            copyNode(root,min);
            prev = root;
            current = root->right;
            leftIsEmpty=1;
            while(current->left!=NULL){
                prev = current;
                current = current->left;
                leftIsEmpty=0;
            }
            if(leftIsEmpty)
                prev->right = current->right;
            else
                prev->left = NULL;
            free(current);
        }
        return;
    }
    current = root;
    while(current->IDno!=id){
        prev = current;
        if(id<current->IDno)
            current = current->left;
        else
            current = current->right;
    }
    if((current->left==NULL) && (current->right==NULL)){
        if(current->IDno < prev->IDno)
            prev->left = NULL;
        else
            prev->right = NULL;
        free(current);
    }
    else if((current->left==NULL) || (current->right==NULL)){
        if((current->IDno < prev->IDno) && (current->right==NULL))
            prev->left = current->left;
        else if((current->IDno < prev->IDno) && (current->left==NULL))
            prev->left = current->right;
        else if((current->IDno > prev->IDno) && (current->right==NULL))
            prev->right = current->left;
        else
            prev->right = current->right;
        free(current);
    }
    else{
        min = findMinimum(current);
        copyNode(current,min);
        prev = current;
        current = current->right;
        leftIsEmpty=1;
        while(current->left!=NULL){
            prev = current;
            current = current->left;
            leftIsEmpty=0;
        }
        if(leftIsEmpty)
            prev->right = current->right;
        else
            prev->left = NULL;
        free(current);
    }
}
//BBBBBBBBBBBBBBBBBBBBBBBBBBBBB

//4444444444444444444444444444
void friends(NODE *tmp){ //*tmp: kullanıcın girdiği ID’ye sahip kişinin düğümü
    int i=0;//Dongu degiskeni
    NODE *friendtmp; //Bulunan arkadaslarin atanacagi dugum
    while(tmp->arkadaslar[i] != -1){
        friendtmp = contains(tmp->arkadaslar[i],0);
        if(friendtmp != NULL)
            printf("%s\n",friendtmp->adSoyad);
        i++;
    }
}
//BBBBBBBBBBBBBBBBBBBBBBBBBBB

//88888888888888888888888888
void printInOrder(NODE *node){ // *node: Yazdırılacak ağacın veya alt ağacın root’u
    if(node != NULL){
        printInOrder(node->left);
        printf("%d - %s\n",node->IDno,node->adSoyad);
        printInOrder(node->right);
    }
}
//BBBBBBBBBBBBBBBBBBBBBBBBBB


//66666666666666666666666666
void printNext(int id){ // Kullanicinin girdigi ID
    NODE *tmp = contains(id,0); // ID'deki dugum
    if(tmp==NULL){
        printf("Kisi bulunamadi\n");
    }
    else{
        printInOrder(tmp->left);
        printInOrder(tmp->right);
    }
}
//BBBBBBBBBBBBBBBBBBBBBBBBB

//7777777777777777777777777
void printGreater(int id,NODE *node){  // id: Kullanicinin girdigi ID,  *node: Yazdırılacak ağacın veya alt ağacın root’u
    if(node!= NULL){
        printGreater(id,node->left);
        if(node->IDno>id)
            printf("%d - %s\n",node->IDno,node->adSoyad);
        printGreater(id,node->right);
    }
}
//BBBBBBBBBBBBBBBBBBBBBBBBB




int main()
{
    NODE *newNode,*tmp;
    // *newNode fonksiyona gonderilmek uzere olusturlan yeni dugumu tutar,
    // *tmp dugumun sistemde olup olmadigini kontrol etmek icin kullanilir
    int id,stop,treecount=0,i,j,k;
    // id: kullanicinin girdigi ID'yi tutmak icin,
    // stop: Dosya okumada durmak icin,
    // treecount: Agactaki dugum sayisini tutar.
    // i ve j: Dongu degiskenleri
    char secim,stop1,buffer[100],tmpc[5];
    // secim: Menu'de kullanicinin secimini tutmak icin.
    // stop1: Menu'ye donme komutundaki while dongusunun durma sartidir.
    // buffer: Dosyadan okunan satirlari tutar.
    // tmpc: Dosyadaki ID'leri gecici olarak tutup daha sonra integera cevirmek icin kullanilir.
    FILE *input;
    // *input: input.txt dosyasinin pointeri
    input = fopen("input.txt","r");
    while(1){
    system("cls");
    printf("Sosyal Ag Sistemine Hosgeldiniz.\nIstediginiz islemin numarasini giriniz.\n\n"
           "0- Dosyadaki herkesi ekle\n"
           "1- Yeni kisi ekle\n"
           "2- Kisi Sil\n"
           "3- Kisi ara\n"
           "4- Kisinin arkadaslarini bul\n"
           "5- Kisi sayisi\n"
           "6- Sonraki kisileri yazdir (printNext)\n"
           "7- ID'si daha buyuk kisileri yazdir (printGreater)\n"
           "8- Herkesi yazdir (printInOrder)\n"
           "e- Cikis");
    secim = getch();
    printf("%c",secim);
    switch(secim){
        case '1':

            //EKLEME BASLA
            system("cls");
            if (input == NULL){
                printf("Dosya açilamadi.");
                exit(0);
            }
            printf("ID giriniz: ");
            scanf("%d",&id);
            if(contains(id,0))
                printf("Eklemek istediginiz kisi sistemde var.\n");
            else{
                fseek(input,0,SEEK_SET);
                stop=0;
                while(!feof(input) && !stop){
                    fgets(buffer,sizeof(buffer),input);
                    newNode = readPerson(buffer,id);
                    if(newNode != NULL){
                        stop=1;
                    }
                }
                if(newNode ==NULL){
                    printf("Eklemek istediginiz ID Input dosyasinda yok.\n");
                }
                else{
                    treecount++;
                    insertNewUser(newNode);
                    printf("%s\n",newNode->adSoyad);
                    printf("ID bulundu ve sisteme eklendi.\n");
                }
            }

            //EKLEME BITIS

            break;
        case '2':
            system("cls");
            printf("ID giriniz. ");
            scanf("%d",&id);
            if(contains(id,0)){
                treecount--;
                deleteUser(id);
                printf("Kisi silindi.\n");
            }
            else
                printf("Silmek istediginiz kisi bulunamadi.\n");
            break;
        case '3':

            system("cls");
            printf("ID giriniz: ");
            scanf("%d",&id);
            tmp = contains(id,1);
            if(tmp!=NULL)
                printf("%s\n",tmp->adSoyad);
            else
                printf("Kisi bulunamadi.\n");

            break;
        case '4':
            system("cls");
            printf("ID giriniz: ");
            scanf("%d",&id);
            tmp = contains(id,0);
            if(tmp != NULL)
                friends(tmp);
            else
                printf("Kisi bulunamadi.\n");
            break;
        case '5':
            system("cls");
            printf("Kullanici sayisi: %d\n",treecount);
            break;
        case '6':
            system("cls");
            printf("ID giriniz: ");
            scanf("%d",&id);
            printNext(id);
            break;
        case '7':
            system("cls");
            printf("ID giriniz: ");
            scanf("%d",&id);
            printGreater(id,root);
            break;
        case '8':
            system("cls");
            printInOrder(root);

            break;
        case '0':
            system("cls");
            fseek(input,0,SEEK_SET);
            j=0;
            while(!feof(input)){
                fgets(buffer,sizeof(buffer),input);
                i=0;
                k=0;
                memset(tmpc,0,sizeof(tmpc));
                if(buffer[i] != '\n'){ // Verilen input.txt dosyasinin sonundaki fazla newline karakterlerini atlamak icin
                    while((buffer[i] < '0' || buffer[i] > '9') && buffer[i] != '\n') //Verilen input.txt dosyasinin ilk satirinda gozukmeyen karakterler bulunnmakta, bu gibi durumlara karsi sayi gelene kadar ilerlemesi icin kontrol koyulmustur
                        i++;
                    while(buffer[i] != ',' && buffer[i] != '\n'){
                        tmpc[k] = buffer[i];
                        i++;
                        k++;
                    }
                    treecount++;
                    newNode = readPerson(buffer,atoi(tmpc));
                    insertNewUser(newNode);
                    j++;
                    if(j%10==0){
                        printInOrder(root);
                        printf("\n");
                    }
                }
            }
            printf("Tum kisiler basariyla eklendi.\n");
            break;
        case 'e':
            system("cls");
            exit(0);
            break;
        default:
            system("cls");
            printf("Lutfen yazili islemlerden seciniz.\n");
        break;
        }
         printf("Menuye donmek icin 'f' tusuna basiniz");
            stop1=0;
            while(!stop1){
                if(getch() == 'f')
                    stop1 = 1;
            }
        }

    return 0;
}
