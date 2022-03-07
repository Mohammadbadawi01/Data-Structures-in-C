#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct date
{
	char day[3];
	char month[3];
	char year[5];
};
struct Node
{
	char firstName[20];
	char lastName[20];
	char gender;
	struct date* dateAdmission;
	struct date* birth;
	char Illness[20];
	char Address[20];
	char typeBlood[3];
	struct Node* left;
	struct Node* right;
	int height;
	int key;
	int flag;
};
int count_patient = 0;

int size_hash = 7;//for size of hash table

int count=0;

/******************************************************************************************************************/
struct Node* createNode(char firstname[20], char lastname[20], char Gender, struct date* dateadmission, struct date* Birth, char illness[20], char address[20], char typeBlood[3]);
struct Node* readFromFileAndInsert(struct Node* root,char nameFile[20],char ch);
struct Node* update(struct Node* root, struct Node* n, struct Node* old);
FILE* saveInFile(struct Node* n, FILE* inFile);
void listLexicographicOrder(struct Node* n);
struct Node* insert(struct Node* root, struct Node* n);
void sortPatientsWithSameIllness(struct Node* root, char Illness[20]);
int MAX(int a, int b);
int height(struct Node* node);
int takeBalance(struct Node* node);
struct Node* rightRotate(struct Node* node);
struct Node* leftRotate(struct Node* node);
struct Node* insert(struct Node* root, struct Node* n);
struct Node* pointNode(struct Node* root);
struct Node* search(struct Node* root, char first[20], char last[20]);

/*                                                     < for hash >                                                       */
struct Node* hashArray;
void insertToTable(struct Node* node);
int getKey(char first[20], char last[20]);
int getIindex(int key);
void printHash(struct Node* arr);
void deleteFromTable(int index);
int searchIntable(char first[20], char last[20]);
FILE* saveInFileHash(FILE* inFile);
/************************************************************************************************************************/
void sortPatientsWithSameIllness(struct Node*root,char Illness[20])
{
	if (root == NULL)
		return;
	if (strcmp(root->Illness, Illness) == 0)
	{
		printf("%s %s %c %s/%s/%s %s/%s/%s %s %s %s\n", root->firstName, root->lastName, root->gender, root->dateAdmission->day, root->dateAdmission->month, root->dateAdmission->year, root->birth->day, root->birth->month, root->birth->year, root->Illness, root->Address, root->typeBlood);
	}
	sortPatientsWithSameIllness(root->left, Illness);
	sortPatientsWithSameIllness(root->right, Illness);
}
FILE* saveInFile(struct Node* n,FILE* inFile)
{
	if (n == NULL)
		return inFile;

	saveInFile(n->left,inFile);
	count++;
	fprintf(inFile, "%s %s#%c#%s%s%s#%s%s%s#%s#%s#%s\n", n->firstName, n->lastName, n->gender, n->dateAdmission->day, n->dateAdmission->month, n->dateAdmission->year, n->birth->day, n->birth->month, n->birth->year, n->Illness, n->Address, n->typeBlood);

	saveInFile(n->right,inFile);
}
int height(struct Node* node)
{
	if(node!=NULL)
		return node->height;
	else
		return 0;
}
int MAX(int a, int b)
{
	if (a > b)
		return a;
	else return b;
}
int takeBalance(struct Node* node)
{
	if (!node)
		return 0;
	else
	return height(node->left) - height(node->right);
}
struct Node* rightRotate(struct Node* node)
{
	struct Node* tmp = node->left;
	struct Node* to = tmp->right;

	tmp->right = node;
	node->left = to;

	node->height = MAX(height(node->left),height(node->right)) + 1;
	tmp->height = MAX(height(tmp->left),height(tmp->right)) + 1;

	return tmp;
}
struct Node* leftRotate(struct Node* node)
{
	struct Node* tmp = node->right;
	struct Node* to = tmp->left;
	
	tmp->left = node;
	node->right = to;

	node->height = MAX(height(node->left),height(node->right)) + 1;
	tmp->height = MAX(height(tmp->left),height(tmp->right)) + 1;

	return tmp;
}
struct Node* insert(struct Node* root, struct Node* n)
{

	if (root == NULL)
	{
		root = n;
		return root;
	}
	
	if (n->firstName[0] < root->firstName[0])
				root->left=insert(root->left,n);
	else if (n->firstName[0] == root->firstName[0])
	{
		if (n->lastName[0] < root->lastName[0])
		{
			root->left = insert(root->left, n);
		}
	}
	else
		root->right=insert(root->right, n);	


	root->height = 1 + MAX(height(root->left),
		height(root->right));

	int BALANCE = takeBalance(root);

	// Left Left Case
	if (BALANCE > 1 && n->firstName[0] < root->left->firstName[0]) return rightRotate(root);

	// Right Right Case
	if (BALANCE < -1 && n->firstName[0] > root->right->firstName[0]) return leftRotate(root);

	// Left Right Case
	if (BALANCE > 1 && n->firstName[0] > root->left->firstName[0])
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Left Case
	if (BALANCE < -1 && n->firstName[0] < root->right->firstName[0])
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}
	return root;
}
struct Node* pointNode(struct Node* root)
{
	struct Node* loop = root;

	for (; loop->left ;)
	{
		loop = loop->left;
	}
	return loop;
}
struct Node* deleteNode(struct Node* root, struct Node *node)
{

	if (root == NULL)
		return root;

	// If the key to be deleted is smaller 
	// than the root's key, then it lies
	// in left subtree 
	if (node->firstName[0] < root->firstName[0])
		root->left = deleteNode(root->left, node);

	// If the key to be deleted is greater 
	// than the root's key, then it lies 
	// in right subtree 
	else if (node->firstName[0] > root->firstName[0])
		root->right = deleteNode(root->right, node);

	// if key is same as root's key, then 
	// This is the node to be deleted 
	else
	{
		// node with only one child or no child 
		if ((root->left == NULL) || (root->right == NULL))
		{
			struct Node* temp = root->left ? root->left :root->right;

			// No child case 
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // One child case 
				*root = *temp; // Copy the contents of 
							  // the non-empty child 
			free(temp);
		}
		else
		{
			// node with two children: Get the inorder 
			// successor (smallest in the right subtree) 

			struct Node* temp = pointNode(root->right);

			// Copy the inorder successor's 
			// data to this node 
			

			strcpy(root->firstName, temp->firstName);
			strcpy(root->lastName, temp->lastName);
			root->gender = temp->gender;
			strcpy(root->dateAdmission->day, temp->dateAdmission->day);
			strcpy(root->dateAdmission->month, temp->dateAdmission->month);
			strcpy(root->dateAdmission->year, temp->dateAdmission->year);
			strcpy(root->birth->day, temp->birth->day);
			strcpy(root->birth->month, temp->birth->month);
			strcpy(root->birth->year, temp->birth->year);
			strcpy(root->Illness, temp->Illness);
			strcpy(root->Address, temp->Address);
			strcpy(root->typeBlood, temp->typeBlood);


			// Delete the inorder successor 
			root->right = deleteNode(root->right,temp);
		}
	}

	// If the tree had only one node
	// then return 
	if (root == NULL)
		return root;

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE 
	root->height = 1 + max(height(root->left),
		height(root->right));

	// STEP 3: GET THE BALANCE FACTOR OF 
	// THIS NODE (to check whether this 
	// node became unbalanced) 
	int balance = takeBalance(root);

	// If this node becomes unbalanced, 
	// then there are 4 cases 

	// Left Left Case 
	if (balance > 1 &&
		takeBalance(root->left) >= 0)
		return rightRotate(root);

	// Left Right Case 
	if (balance > 1 && takeBalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Right Case 
	if (balance < -1 && takeBalance(root->right) <= 0)
		return leftRotate(root);

	// Right Left Case 
	if (balance < -1 && takeBalance(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}
struct Node* search(struct Node* root,char first[20],char last[20])
{
	if (root == NULL)
		return NULL;
	if ((strcmp(root->firstName, first) == 0)&& (strcmp(root->lastName, last) == 0))
	{
		return root;
	}
	if ((strcmp(root->firstName, first) == -1) && (strcmp(root->lastName, last) == -1))
	{
		search(root->left, first, last);
	}
	if ((strcmp(root->firstName, first) == 1) && (strcmp(root->lastName, last) == 1))
	{
		search(root->right, first, last);
	}
	return NULL;
}
struct Node* update(struct Node* root,struct Node *n, struct Node* old)
{
	if (root == NULL)
		return NULL;
	if (root->gender == old->gender && strcmp(root->firstName, old->firstName) == 0 && strcmp(root->lastName, old->lastName) == 0)
	{
		n->left = root->left;
		n->right = root->right;
		root = n;
		return root;
	}
	update(root->left, n,old);
	update(root->right, n,old);
	return NULL;
}
struct Node *readFromFileAndInsert(struct Node* root,char name[20],char ch)
{
	FILE* readFile;
	char line[100];

	if ((readFile = fopen(name, "r")) == NULL)
	{
		printf("\ncant open file to read patients !!\n");
		exit(1);
	}
	int i = 0;
	int index = 0;
	int indexHash = 0;
	int x = 0;
	while (x=fscanf(readFile, "%[^\n]\n", line) != EOF)
	{
		if (x == '\n')
			break;
		
		struct Node* n=NULL;
		n= malloc(sizeof(struct Node));

		n->birth = NULL;
		n->dateAdmission = NULL;
		n->birth = malloc(sizeof(struct date));
		n->dateAdmission = malloc(sizeof(struct date));


		for (i = 0; line[i] != ' '; i++)
		{
			n->firstName[i] = line[i];
		}
		n->firstName[i] = '\0';

		for (index = 0, i++; line[i] != '#'; i++, index++)
		{
			n->lastName[index] = line[i];
		}
		n->lastName[index] = '\0';

		n->gender = line[++i];
		i += 2;

		n->dateAdmission->day[0] = line[i];
		n->dateAdmission->day[1] = line[++i];
		n->dateAdmission->day[2] = '\0';

		n->dateAdmission->month[0] = line[++i];
		n->dateAdmission->month[1] = line[++i];
		n->dateAdmission->month[2] = '\0';

		n->dateAdmission->year[0] = line[++i];
		n->dateAdmission->year[1] = line[++i];
		n->dateAdmission->year[2] = line[++i];
		n->dateAdmission->year[3] = line[++i];
		n->dateAdmission->year[4] = '\0';

		i += 2;

		n->birth->day[0] = line[i];
		n->birth->day[1] = line[++i];
		n->birth->day[2] = '\0';

		n->birth->month[0] = line[++i];
		n->birth->month[1] = line[++i];
		n->birth->month[2] = '\0';

		n->birth->year[0] = line[++i];
		n->birth->year[1] = line[++i];
		n->birth->year[2] = line[++i];
		n->birth->year[3] = line[++i];
		n->birth->year[4] = '\0';

		for (index = 0, i += 2; line[i] != '#'; i++, index++)
		{
			n->Illness[index] = line[i];
		}
		n->Illness[index] = '\0';

		for (i++, index = 0; line[i] != '#'; index++, i++)
		{
			n->Address[index] = line[i];
		}
		n->Address[index] = '\0';
		for (index = 0, i++; line[i] != 0; i++, index++)
		{
			n->typeBlood[index] = line[i];
			n->typeBlood[index + 1] = '\0';
		}
		n->typeBlood[2] = '\0';
		if (ch == 'P')
		{
			count_patient++;
			root = insert(root, createNode(n->firstName, n->lastName, n->gender, n->dateAdmission, n->birth, n->Illness, n->Address, n->typeBlood));
		}
		else if (ch=='H')
		{
			n->key = getKey(n->firstName,n->lastName);

			insertToTable(createNode(n->firstName, n->lastName, n->gender, n->dateAdmission, n->birth, n->Illness, n->Address, n->typeBlood));
			indexHash++;
		}
		//printf("%s %s %c %s/%s/%s %s/%s/%s %s %s %s\n", n->firstName, n->lastName, n->gender, n->dateAdmission->day, n->dateAdmission->month, n->dateAdmission->year, n->birth->day, n->birth->month, n->birth->year, n->Illness, n->Address, n->typeBlood);
	}
	fclose(readFile);
	return root;
}
void listLexicographicOrder(struct Node* root)
{
	if (root == NULL)
		return;

	listLexicographicOrder(root->left);
	printf("%s %s %c %s/%s/%s %s/%s/%s %s %s %s\n", root->firstName, root->lastName, root->gender, root->dateAdmission->day, root->dateAdmission->month, root->dateAdmission->year, root->birth->day, root->birth->month, root->birth->year, root->Illness, root->Address, root->typeBlood);
	listLexicographicOrder(root->right);
}
struct Node* createNode(char firstname[20],char lastname[20],char Gender,struct date* dateadmission,struct date* Birth,char illness[20],char address[20],char typeBlood[3])
{
	struct Node* n = NULL;
	n = malloc(sizeof(struct Node));
	strcpy(n->firstName, firstname);
	strcpy(n->lastName, lastname);
	n->gender=Gender;
	n->dateAdmission = dateadmission;
	n->birth = Birth;
	strcpy(n->Address, address);
	strcpy(n->typeBlood,typeBlood);
	strcpy(n->Illness,illness);
	n->left = NULL;
	n->right = NULL;
	n->height = 1;
	n->key = -1;
	return n;
}
/**************************************************************************************************************************/




int getKey(char first[20], char last[20]) 
{
	int i;
	int key = 0;
	for (i = 0; first[i] != '\0'; i++)
	{
		key += first[i];
	}
	for (i = 0; last[i] != '\0'; i++)
	{
		key += last[i];
	}
	return key;
}
int getIindex(int key)
{
	return key%count_patient;
}
void insertToTable(struct Node*node)
{
	int key = getKey(node->firstName, node->lastName);
	int index = 0;
	index = getIindex(key);
	
	
	if (hashArray[index].key == -1)
	{
		hashArray[index] = *node;
		hashArray[index].key = key;
	}
	else
	{
		for (int i = 1; i <= size_hash; i++)
		{
			++index;
			index = index % size_hash;
			if (hashArray[index].key == -1)
			{
				hashArray[index] = *node;
				hashArray[index].key = key;
				break;
			}
		}
	}
	
}
void deleteFromTable(int index)
{
	hashArray[index].key = -1;
}
int searchIntable(char first[20],char last[20])
{

	int index=0;
	index = getIindex(getKey(first, last));

	if (hashArray[index].key != -1)
	{
		return 1;
	}
	else
		return 0;
}
FILE* saveInFileHash(FILE* inFile)
{
	for (int i = 0; i < size_hash; i++)
	{
		if(hashArray[i].key!=-1)
		fprintf(inFile, "%s %s#%c#%s%s%s#%s%s%s#%s#%s#%s\n", hashArray[i].firstName, hashArray[i].lastName, hashArray[i].gender, hashArray[i].dateAdmission->day, hashArray[i].dateAdmission->month, hashArray[i].dateAdmission->year, hashArray[i].birth->day, hashArray[i].birth->month, hashArray[i].birth->year, hashArray[i].Illness, hashArray[i].Address, hashArray[i].typeBlood);
	}
	return inFile;
}
void printHash(struct Node* arr)
{
	for (int i = 0; i < size_hash; i++)
	{
		if (arr[i].key == -1)
		{
			printf("           _________________________________________________________________________________\n");
			printf("index %d:: |________________________________________________________________________________|\n\n", i);
		}
		else
			printf("index %d:: %s %s %c %s/%s/%s %s/%s/%s %s %s %s\n\n", i, arr[i].firstName, arr[i].lastName, arr[i].gender, arr[i].dateAdmission->day, arr[i].dateAdmission->month, arr[i].dateAdmission->year, arr[i].birth->day, arr[i].birth->month, arr[i].birth->year, arr[i].Illness, arr[i].Address, arr[i].typeBlood);

	}
}

int main()
{	

	struct Node* root=NULL;
	struct Node* n = NULL;
	n = malloc(sizeof(struct Node));
	n->dateAdmission = NULL;
	n->dateAdmission = malloc(sizeof(struct date));
	n->birth = NULL;
	n->birth = malloc(sizeof(struct date));
	n->key = -1;
	char firstName[20];
	char lastName[20];
	char Gender=' ';

	while (1)
	{	
		printf("\n                  +-------------------------------------------------------------------------------+");
		printf("\n                  |<1> Enter 1 to Read the file                                                   |");
		printf("\n                  |<2> Enter 2 to Insert a new patient                                            |");
		printf("\n                  |<3> Enter 3 to update the information of the patient                           |");
		printf("\n                  |<4> Enter 4 to List all patients in lexicographic order with their associated  |");
		printf("\n                  |<5> Enter 5 to List all patients that have the same illness                    |");
		printf("\n                  |<6> Enter 6 to Delete a patient from the system                                |");
		printf("\n                  |<7> Enter 7 to Save                                                            |");
		printf("\n                  |<0> Enter 0 to exit                                                            |");
		printf("\n                  +-------------------------------------------------------------------------------+\n");
		printf("                                   Enter option: ");
		
		int dum = 0;
		int menu;
		dum = scanf("%d", &menu);

		if (menu == 0)
			break;
		
		switch (menu)
		{
		case 2: {

			printf("\nEnter the info of pationt ->");
			printf("\nEnter first name: ");
			dum = scanf("%s", n->firstName);
			printf("\nEnter last name: ");
			dum = scanf("%s", n->lastName);
			printf("\nEnter Gender: ");
			dum = scanf("%s", &n->gender);
			printf("\nEnter Date of admission dd/mm/yyyy: ");
			dum = scanf("%s%s%s", n->dateAdmission->day, n->dateAdmission->month, n->dateAdmission->year);
			printf("\nEnter Date of birth dd/mm/yyyy: ");
			dum = scanf("%s%s%s", n->birth->day, n->birth->month, n->birth->year);
			printf("\nEnter Illness: ");
			dum = scanf("%s", n->Illness);
			printf("\nEnter Address(City): ");
			dum = scanf("%s", n->Address);
			printf("\nEnter Blood type: ");
			dum = scanf("%s", n->typeBlood);
			count_patient++;
			root = insert(root, createNode(n->firstName, n->lastName, n->gender, n->dateAdmission, n->birth, n->Illness, n->Address, n->typeBlood));
			break;
		}
		case 1: {
			root = readFromFileAndInsert(root, "Patients.txt",'P');
			printf("\nReaded succesfuly $\n");
			break;
		}
		case 3: {
			printf("\nEnter first name: ");
			dum = scanf("%s", firstName);
			printf("\nEnter last name: ");
			dum = scanf("%s", lastName);
			struct Node* ser = NULL;
			ser = malloc(sizeof(struct Node));
			if (ser = search(root, firstName, lastName))
			{
				struct Node* old = NULL;
				old = malloc(sizeof(struct Node));
				old = ser;
				while (1)
				{
					
					printf("\n<> Enter 1 to chagnch first name");
					printf("\n<> Enter 2 to chagnch last name");
					printf("\n<> Enter 3 to chagnch gender");
					printf("\n<> Enter 4 to chagnch Date of admission");
					printf("\n<> Enter 5 to chagnch Date of birth");
					printf("\n<> Enter 6 to chagnch Illness");
					printf("\n<> Enter 7 to chagnch Address");
					printf("\n<> Enter 8 to chagnch Blood type");
					printf("\n<> Enter 0 exit");

					printf("\n\nEnter your option: ");
					dum = scanf("%d", &menu);
					if (menu == 0)break;
					switch (menu)
					{
					case 1: {
						printf("\nEnter first name: ");
						dum = scanf("%s", ser->firstName);
						break;
					}
					case 2: {
						printf("\nEnter last name: ");
						dum = scanf("%s", ser->lastName);
						break;
					}
					case 3: {
						printf("\nEnter Gender: ");
						dum = scanf("%s", &ser->gender);
						break;
					}
					case 4: {
						printf("\nEnter Date of admission dd/mm/yyyy: ");
						dum = scanf("%s%s%s", ser->dateAdmission->day, ser->dateAdmission->month, ser->dateAdmission->year);

						break;
					}
					case 5: {
						printf("\nEnter Date of birth dd/mm/yyyy: ");
						dum = scanf("%s%s%s", ser->birth->day, ser->birth->month, ser->birth->year);

						break;
					}
					case 6: {
						printf("\nEnter Illness: ");
						dum = scanf("%s", ser->Illness);

						break;
					}
					case 7: {
						printf("\nEnter Address(City): ");
						dum = scanf("%s", ser->Address);

						break;
					}
					case 8: {
						printf("\nEnter Blood type: ");
						dum = scanf("%s", ser->typeBlood);
						break;
					}
					}
				}
				root = update(root, ser, old);
			}
			else
			{
				printf("\npationt not found !!\n");
			}
			break;
		}
		case 4: {
			printf("\n");
			if (root == NULL)
				printf("\nEmpty system");
			else
			listLexicographicOrder(root);
			printf("\n");
			break;
		}
		case 5: {
			printf("\nEnter the Illnes: ");
			char ill[20];
			dum=scanf("%s", ill);
			printf("\n");
			sortPatientsWithSameIllness(root,ill);
			break;
		}
		case 6: {
			printf("\nEnter first name: ");
			dum = scanf("%s", firstName);
			printf("\nEnter last name: ");
			dum = scanf("%s", lastName);
			struct Node* found = malloc(sizeof(struct Node));

			if (found=search(root, firstName, lastName))
			{
				count_patient--;
				root = deleteNode(root, found);
				printf("\ndeleted pationt $$\n");
			}
			else
			{
				printf("\npationt not found !!\n");
			}
			break;
		}
		case 7: {
			FILE* inFile = fopen("patients_hash.data", "w");
			inFile=saveInFile(root, inFile);
			fclose(inFile);
			
			hashArray = (struct Node*)calloc(size_hash, sizeof(struct Node));

			for (int i = 0; i < size_hash; i++)
			{
				hashArray[i].key = -1;
			}

		    readFromFileAndInsert(hashArray, "patients_hash.data", 'H');

			while (1)
			{
				printf("\n                  +\x1B[91m------------------------------(menu for hash)----------------------------------\033[0m+");
				printf("\n                  |<1> Enter 1 to Print hashed table                                              |");
				printf("\n                  |<2> Enter 2 to Print out table size                                            |");
				printf("\n                  |<3> Enter 3 to Print out the used hash function                                |");
				printf("\n                  |<4> Enter 4 to Insert a new record into the hash table                         |");
				printf("\n                  |<5> Enter 5 to Search for a specific patient                                   |");
				printf("\n                  |<6> Enter 6 to Delete a specific record                                        |");
				printf("\n                  |<7> Enter 7 to Save hash table back to file                                    |");
				printf("\n                  |<0> Enter 0 to exit                                                            |");
				printf("\n                  +\x1B[91m-------------------------------------------------------------------------------\033[0m+\n");
				printf("                                   Enter option: ");

				dum=scanf("%d", &menu);
				if (menu == 0)
					break;
				if (menu == 1)
				{
					printf("\n");
					printHash(hashArray);
				}
				else if (menu==2)
				{
					printf("\nsize: %d\n", size_hash);
				}
				else if (menu==3)
				{
					printf("\nKey(string): sum of char +string[i]");
					printf("\nIndex(key): key%%size");
					printf("\nH(key): array[Index]=item");
					printf("\ntry:\nwhile(index!=-1)");
					printf("\n   : (++index)=index%%size");
					printf("\n   : array[Index]=item");
				}
				else if (menu == 4)
				{
					if (count > size_hash)
					{

						printf("\nFull hash table !! %d \n",count);
					}
					else
					{
						printf("\nEnter the info of pationt ->");
						printf("\nEnter first name: ");
						dum = scanf("%s", n->firstName);
						printf("\nEnter last name: ");
						dum = scanf("%s", n->lastName);
						printf("\nEnter Gender: ");
						dum = scanf("%s", &n->gender);
						printf("\nEnter Date of admission dd/mm/yyyy: ");
						dum = scanf("%s%s%s", n->dateAdmission->day, n->dateAdmission->month, n->dateAdmission->year);
						printf("\nEnter Date of birth dd/mm/yyyy: ");
						dum = scanf("%s%s%s", n->birth->day, n->birth->month, n->birth->year);
						printf("\nEnter Illness: ");
						dum = scanf("%s", n->Illness);
						printf("\nEnter Address(City): ");
						dum = scanf("%s", n->Address);
						printf("\nEnter Blood type: ");
						dum = scanf("%s", n->typeBlood);
						insertToTable(n);
						count++;
					}

				}
				else if (menu == 5)
				{
					printf("\nEnter first name: ");
					dum = scanf("%s", firstName);
					printf("\nEnter last name: ");
					dum = scanf("%s", lastName);

					if (searchIntable(firstName, lastName))
					{
						printf("\nFound patients\n");
					}
					else
					{
						printf("\nDont found patients\n");

					}
				}
				else if (menu == 6)
				{
					printf("\nEnter first name: ");
					dum = scanf("%s", firstName);
					printf("\nEnter last name: ");
					dum = scanf("%s", lastName);

					if (searchIntable(firstName, lastName))
					{
						deleteFromTable(getIindex(getKey(firstName, lastName)));
						printf("\nDelete succesfly $ \n");
					}
					else
					{
						printf("\nDont found patients\n");
					}

					
				}
				else if (menu == 7)
				{
					FILE* saveHash;
					saveHash = fopen("patients_hash.data", "w");
					saveHash=saveInFileHash(saveHash);
					fclose(saveHash);
				}
				else 
				{
					printf("\nEnter valid choose !! \n");
				}
			}
		}
		}
	}

}
