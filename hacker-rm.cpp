#include <fcntl.h>      
#include <unistd.h>    
#include <sys/stat.h>  
#include <string.h>    
#include <errno.h>     

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        const char *msg = "Please provide a file path to delete.\n";
        write(STDERR_FILENO, msg, strlen(msg)); 
        return 1;
    }

    const char *filePath = argv[1];

   
    int fd = open(filePath, O_WRONLY);
    if (fd == -1)
    {
        const char *errorMsg = "Error: Cannot open file. ";
        write(STDERR_FILENO, errorMsg, strlen(errorMsg));
        write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
        write(STDERR_FILENO, "\n", 1);
        return 1;
    }

   
    struct stat fileStat;
    if (fstat(fd, &fileStat) == -1)
    {
        const char *errorMsg = "Error: Cannot access file size. ";
        write(STDERR_FILENO, errorMsg, strlen(errorMsg));
        write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
        write(STDERR_FILENO, "\n", 1);
        close(fd);
        return 1;
    }

    off_t fileSize = fileStat.st_size;

    
    char nullByte = '\0';
    for (off_t i = 0; i < fileSize; ++i)
    {
        if (write(fd, &nullByte, 1) == -1)
        {
            const char *errorMsg = "Error: Failed to overwrite file content. ";
            write(STDERR_FILENO, errorMsg, strlen(errorMsg));
            write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
            write(STDERR_FILENO, "\n", 1);
            close(fd);
            return 1;
        }
    }

    close(fd); 

   
    if (unlink(filePath) == -1)
    {
        const char *errorMsg = "Error: Failed to delete the file. ";
        write(STDERR_FILENO, errorMsg, strlen(errorMsg));
        write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
        write(STDERR_FILENO, "\n", 1);
        return 1;
    }

    const char *successMsg = "File was securely deleted.\n";
    write(STDOUT_FILENO, successMsg, strlen(successMsg)); 
    return 0;
}
