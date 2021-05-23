#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>

//Function to check if the file exists or not
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

//Function to move file
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

//Recursive List 
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

//Main Function
int main(int argc, char *argv[])
{
  char cwd[1000];

  //3A - For argument -f
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

  else
  {
    //3C - For argument *
     if (strcmp(argv[1], "*") == 0)
    {
      if (getcwd(cwd, sizeof(cwd)) != NULL)
      {
        //Open the working directory
        listFilesRecursively(cwd);
      }
    }

    //3B - For argument -d
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
    }
  }
}
