# soal-shift-sisop-modul-3-I07-2021

Made by:

Group I07

Gede Yoga Arisudana (05111942000009)

Salma Rahma Lailia (05111942000016)

Zulfiqar Rahman Aji (05111942000019)



## PROBLEM 1

## PROBLEM 2

a. We want to Create a matrix multiplication program (4x3 and 3x6) and then display the results. The matrix will contain the numbers 1-20. In this time we need shared memory to give acces soal2b.c to input the matrix calculation result.

```
 int a[4][3],b[3][6];
    
    printf("---Input Matrix a and b---\n");
    for(int i=0; i<4;i++){
    	for(int j=0;j<3;j++){
    		scanf("%d",&a[i][j]);
    	}
    }
    
    for(int i=0; i<3;i++) {
    	for(int j=0;j<6;j++) {
    		scanf("%d",&b[i][j]);
    	}
    }
```

Then shared the memory

```
    int multiply[4][6];

    for(int i=0; i<4; i++) {
        for(int j=0; j<6; j++) {
            for(int k=0; k<3; k++) {
                sum += arr1[i][k] * arr2[k][j];
            }
            multiply[i][j] = sum;
            sum = 0;
        }
    }
    
    /*calculate the result a*b*/

    for(int i=0; i<4; i++) {
        for(int j=0; j<6; j++){
            value[i*6+j] = multiply[i][j]; 
            printf("%d\t", multiply[i][j]);
        } 
        
        printf("\n");
    }
    
    /*print result in 1 matrix*/

    shmdt(value);
}
```

b. Because we want to input result from soal2a.c so we have to use shared memory to give an acces to soal2b.c. Then calculation for the existing matrix. The calculation is that each cell originating from matrix A becomes a number for factorial, then cells from matrix B become the maximum factorial limit.

```
   // Input New Matrix
    printf("---Input New Mat---\n");
    for(int i=0; i<24; i++) {
        scanf("%d", &New_mat[i]);
    }

    printf("\n");

    printf("---Matrix from Soal2a.c---");
    for(int i=0; i<24; i++) {
        if(i%6 == 0){
            printf("\n");
        }
        
        printf("%d ", value[i]);
    }
```

```
 //print value from soal2a.c

    printf("\n\n");
    
    for(unsigned long long i=0; i<24; i++) {
       if(New_mat[i] == 0 || value[i] == 0) {
           result[i]=0;
       }
       
       else if(value[i] == 1) {
           result[i] = 1; 
       }
       
       else if(value[i] == 2) {
           result[i] = 2;
       }
       
       else {
           if(value[i] >= New_mat[i]) {
               result[i] = value[i];
               for(unsigned long long a=1; a<New_mat[i]; a++){
                   result[i] = result[i] * (value[i]-a); 
               }
           }
           
           else {
               result[i] = value[i];
               for(unsigned long long a=1; a<value[i]; a++){
                   result[i] = result[i] * (value[i]-a);  
               }
           }
       }
    }
```

c. check the top 5 processes consuming computer resources with the command  “ps aux | sort -nrk 3,3 | head -5”

```
char* cmd1[] = {"ps", "aux", NULL};
char* cmd2[] = {"sort", "-nrk", "3,3", NULL};
char* cmd3[] = {"head", "-5", NULL};

int pid;
int pipe1[2];
int pipe2[2];

void exec1() {
	dup2(pipe1[1], 1);

	close(pipe1[0]);
	close(pipe1[1]);

	execv("/bin/ps", cmd1);
}

void exec2() {
	dup2(pipe1[0], 0);
	dup2(pipe2[1], 1);

	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[0]);
	close(pipe2[1]);

	execv("/bin/sort", cmd2);
}

void exec3() {
	dup2(pipe2[0], 0);

	close(pipe2[0]);
	close(pipe2[1]);

	execv("/bin/head", cmd3);
}
```

## PROBLEM 3

To do problem number 3, I will first create a function named `checkfile` to check if the file is exist or not
```
int checkfile(const char *filename)
{
  struct stat buffer;
  int exist = stat(filename, &buffer);
  
  if(exist == 0)
  {
    return 1;
  }

  else
  {
    return 0;
  }
}
```
After that, I also create a function to move the file using a function named `*move` 
```
void *move(void *filename)
{
  int i;
  char cwd[1000];
  char dirname[1000]; 
  char hidden[1000]; 
  char hiddenname[1000]; 
  char file[1000]; 
  char existsfile[1000];

  strcpy(existsfile, filename);
  strcpy(hiddenname, filename);
  char *hname = strrchr(hiddenname, '/');
  strcpy(hidden, hname);

  //3D - For hidden files
  if(hidden[1] == '.')
  {
    strcpy(dirname, "Hidden");
  }

  //Normal files or files with extension
  else if(strstr(filename, ".") != NULL)
  {
    strcpy(file, filename);
    strtok(file, ".");
    char *token = strtok(NULL, "");

    //For insensitive case
    for(i = 0; token[i]; i++)
    {
      token[i] = tolower(token[i]);
    }

    strcpy(dirname, token);
  }

  //3D - For files without extension
  else
  {
    strcpy(dirname, "Unknown");
  }

  //To check if the files exist or not
  int exist = checkfile(existsfile);

  if(exist)
  {
    mkdir(dirname, 0755);
  }
    
  //Function to get the file name
  if (getcwd(cwd, sizeof(cwd)) != NULL)
  {
    char *name = strrchr(filename, '/');
    char namefile[1000];
    strcpy(namefile, cwd);
    strcat(namefile, "/");
    strcat(namefile, dirname);
    strcat(namefile, name);

    //Function to move the file
    rename(filename, namefile);
  }
}
```
From the function above, we will categorize the file according to each of their extension, and if there is no extension the file will be input to the `Unknown` folder and the hidden file will be input to the `Hidden` folder like this
```
if(hidden[1] == '.')
{
  strcpy(dirname, "Hidden");
}

else
{
  strcpy(dirname, "Unknown");
}
```
And for the file with extension, I use `tolower` function because this program is case insensitive, like this
```
for(i = 0; token[i]; i++)
{
  token[i] = tolower(token[i]);
}
```
In the `*move` function, I also make a function to take the file name using `strrchr` function and to move the file using `rename` function, like this
```
if (getcwd(cwd, sizeof(cwd)) != NULL)
{
  char *name = strrchr(filename, '/');
  char namefile[1000];
  strcpy(namefile, cwd);
  strcat(namefile, "/");
  strcat(namefile, dirname);
  strcat(namefile, name);

  rename(filename, namefile);
}
```
After the `*move` function, I will make `listFilesRecursively` function because we need to list the file recursively for number 3B and 3C. After we list the file, we will make thread and use the `*move` function as the argument and after making the thread, we will join it using `pthread_join`
```
void listFilesRecursively(char *basePath)
{
  int n = 0;
  char path[1000];
  struct dirent *dp;
  struct stat buffer;
  DIR *dir = opendir(basePath);

  if(!dir)
  {
    return;
  }

  while((dp = readdir(dir)) != NULL)
  {
    if(strcmp(dp -> d_name, ".") != 0 && strcmp(dp -> d_name, "..") != 0)
    {
      //Construct new path from our base path
      strcpy(path, basePath);
      strcat(path, "/");
      strcat(path, dp -> d_name);

      if(stat(path, &buffer) == 0 && S_ISREG(buffer.st_mode))
      {
        //Make thread to check move function
        pthread_t thread;
        int err = pthread_create(&thread, NULL, move, (void *)path);
        pthread_join(thread, NULL);
      }

      listFilesRecursively(path);
    }
  }

  closedir(dir);
}
```
Now, into the problem 3A, 3B, 3C, 3D, and 3E

**a. Make the program accepts -f option, so the user may add file arguments to be categorized as much as they want**
```
if (strcmp(argv[1], "-f") == 0)
  {
    pthread_t thread;
    int i;

    //Make thread and output
    for (i = 2; i < argc; i++)
    {
        char message[1000];
        int exist = checkfile(argv[i]);
        if (exist)
        {
          sprintf(message, "File %d : Berhasil Dikategorikan", i - 1);
        }

        else
        {
          sprintf(message, "File %d : Sad, gagal :(", i - 1);
        }
            
        printf("%s\n", message);
        int err = pthread_create(&thread, NULL, move, (void *)argv[i]);
    }

    //Join all thread
    pthread_join(thread, NULL);
  }
```
In problem 3A, we are needed to make the program accepts -f option, so the user may add file arguments to be categorized as much as they want, so the first thing that I will do, I use `strcmp` function so it can accept the -f option, after that I will make thread and output message according to the problem, then I use `int err` function to make new thread so it can move the file, lstly join the all the thread with `pthread_jon` function.

Output:
![Screenshot from 2021-05-23 21-21-44](https://user-images.githubusercontent.com/74660281/119264758-97f65780-bc0e-11eb-9b8d-cc0a6b4f87ed.png)

**b. Make the program accepts -d option, so the user may only input 1 directory as it's arguments**
```
 else if (strcmp(argv[1], "-d") == 0)
    {
      //Open directory according to the second argument
      listFilesRecursively(argv[2]);
      struct stat buffer;
      int err = stat(argv[2], &buffer);

      if(err == -1)
      {
        printf("Yah, gagal disimpan :(\n");
      }

      else
      {
        printf("Direktori sukses disimpan!\n");
      }
```
In problem 3B, we are needed to make the program accepts -d option, so the user may only input 1 directory as it's arguments, so the first thing that I will do, I will use `strcmp` function so it can accept the -d option, then I will use `listFilesRecursively` function, to open the directory and move the file, after that I use `struct stat` function to read all the file and to put it in the `buffer`, then I use `int err` function produces the message.

Output:
![Screenshot from 2021-05-23 21-22-41](https://user-images.githubusercontent.com/74660281/119264817-dc81f300-bc0e-11eb-822e-f65a43291167.png)

**c. Make the program accepts \* option, so it will categorizes all the file in the working directory when the C program is run**
```
if (strcmp(argv[1], "*") == 0)
    {
      if (getcwd(cwd, sizeof(cwd)) != NULL)
      {
        //Open the working directory
        listFilesRecursively(cwd);
      }
    }
```
In problem 3C, we are needed to make the program accepts \* option, so it will categorizes all the file in the working directory when the C program is run, so the first thing I will do, I will use `strcmp` function so it can accept the \* option, then I will use `getcwd` function to know the file location and I will use `listFilesRecursively` function to list the file recursively.

Output:
![Screenshot from 2021-05-23 21-23-50](https://user-images.githubusercontent.com/74660281/119264824-e0157a00-bc0e-11eb-8982-d9f9bfb948b6.png)

**d. All files must be moved into a folder**
```
if(hidden[1] == '.')
{
  strcpy(dirname, "Hidden");
}

else
{
  strcpy(dirname, "Unknown");
}
```
In problem 3D, we are needed to make all files to be moved into a folder, this problem is answered by answering problem 3A, 3B, and 3C. And the function above is for the `hidden` and `unknown` files.

**e. Each file to be categorized will be processed by a thread to make the program run in parallel to make it quicker**

In problem 3E, we are needed to categorized each file so it will be processed by a thread to make the program run in parallel to make it quicker, therefore I make thread according to the number of files that will be categorized and then list each thread that is run.
