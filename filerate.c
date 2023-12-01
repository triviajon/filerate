#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int MAX_FILES = 1024;

// Assumes file is open for reading
void _get_filesize(FILE *file, long *filsize)
{
  fseek(file, 0, SEEK_END);
  *filsize = ftell(file);
}

// Places filesize of filename into filesize
void get_filesize(const char *filename, long *filesize)
{
  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  _get_filesize(file, filesize);

  fclose(file);
}

// Places rate of growth (in bytes per second) of filename[i] into bytes_per_second[i]
void get_rates(char filenames[MAX_FILES][FILENAME_MAX], long bytes_per_second[MAX_FILES], int num_files) {
  FILE *files[MAX_FILES];

  for (int filenum = 0; filenum < num_files; filenum++)
  {
    char *filename = filenames[filenum];
    files[filenum] = fopen(filename, "rb");
    if (files[filenum] == NULL)
    {
      perror("Error opening file");
      exit(EXIT_FAILURE);
    }
  }

  long size_before[MAX_FILES];
  long size_after[MAX_FILES];

  for (int filenum = 0; filenum < num_files; filenum++)
  {
    _get_filesize(files[filenum], &size_before[filenum]);
  }

  sleep(1);

  for (int filenum = 0; filenum < num_files; filenum++)
  {
    _get_filesize(files[filenum], &size_after[filenum]);
    fclose(files[filenum]);

    bytes_per_second[filenum] = size_after[filenum] - size_before[filenum];
  }
}

// Prints the help information
void print_help(const char *program_name)
{
  printf("Utility to monitor the file size of multiple files. \n");
  printf("Usage: %s [-f|--follow] [-r|--rate] [FILE]+\n", program_name);
  printf("Options:\n");
  printf("  -f, --follow   Continuously monitor changes in file size\n");
  printf("  -r, --rate     Display the rate of increase of file size\n");
  printf("  -h  --help     Display this help message\n");
}

int main(int argc, char *argv[])
{
  if (argc < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
  {
    print_help(argv[0]);
    exit(EXIT_SUCCESS);
  }

  char filenames[MAX_FILES][FILENAME_MAX];
  int follow = 0; // flag for following changes
  int rate = 0;   // flag for getting rate of increase
  int num_files = 0;

  // Check for optional parameter "-f" or "--follow"
  for (int i = 1; i < argc; ++i)
  {
    if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--follow") == 0)
    {
      follow = 1;
    }
    else if ((strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--rate") == 0))
    {
      rate = 1;
    }
    else
    {
      strcpy(filenames[num_files++], argv[i]);
    }
  }

  if (num_files == 0)
  {
    fprintf(stderr, "No files provided.\n");
    exit(EXIT_FAILURE);
  }


  if (rate)
  {
    long bytes_per_second[MAX_FILES];
    get_rates(filenames, bytes_per_second, num_files);
    for (int filenum = 0; filenum < num_files; filenum++)
    {
      printf("%s: %lu bytes per second\n", filenames[filenum], bytes_per_second[filenum]);
    }

    if (follow)
    {
      while (1)
      {
        printf("\033[H\033[J"); // Clear the console screen
        long bytes_per_second[MAX_FILES];
        get_rates(filenames, bytes_per_second, num_files);
        for (int filenum = 0; filenum < num_files; filenum++)
        {
          printf("%s: %lu bytes per second\n", filenames[filenum], bytes_per_second[filenum]);
        }
        fflush(stdout);
      }
    }
  }
  else
  {
    long filesize;
    for (int filenum = 0; filenum < num_files; filenum++)
    {
      char *filename = filenames[filenum];
      get_filesize(filename, &filesize);
      printf("%s: %lu bytes\n", filename, filesize);
    }

    if (follow)
    {
      while (1)
      {
        sleep(1);
        printf("\033[H\033[J"); // Clear the console screen
        for (int filenum = 0; filenum < num_files; filenum++)
        {
          char *filename = filenames[filenum];
          get_filesize(filename, &filesize);
          printf("%s: %lu bytes\n", filename, filesize);
        }
        fflush(stdout);
      }
    }
  }

  printf("\n");

  return 0;
}
