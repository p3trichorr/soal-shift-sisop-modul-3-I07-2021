# soal-shift-sisop-modul-3-I07-2021

Made by:

Group I07

Gede Yoga Arisudana (05111942000009)

Salma Rahma Lailia (05111942000016)

Zulfiqar Rahman Aji (05111942000019)



## PROBLEM 1

## PROBLEM 2

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
And for the file with extension, I use `tolower` command because this program is case insensitive, like this
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
