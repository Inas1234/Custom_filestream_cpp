#include <iostream>
#include <cmath>
#include <cstring>
#include <fcntl.h>

class FileManager{
  public:
    FileManager(const char* fileName, int appendMode = 0) {
        int flags = O_WRONLY;

        if (appendMode) {
            flags |= O_APPEND;
        } else {
            flags |= O_TRUNC;
        }

        mode_t mode = 0666;  

        asm volatile (
            "mov $2, %%rax;"          
            "syscall;"
            : "=a" (fileDescriptor)
            : "D" (fileName), "S" (flags), "d" (mode)
            : "rcx", "r11", "memory"
        );
    };

    ~FileManager() {
        asm volatile (
            "mov $3, %%rax;"          
            "syscall;"
            :
            : "D" (fileDescriptor)
            : "rax", "rcx", "r11", "memory"
        );
    };


    void write(const char* text) {
        asm volatile (
            "movq %[text], %%rsi;"    
            "xorq %%rcx, %%rcx;"      
        "count_loop:"
            "cmpb $0, (%%rsi);"       
            "je end_count;"           
            "incq %%rsi;"             
            "incq %%rcx;"             
            "jmp count_loop;"         
        "end_count:"
            "mov $1, %%rax;"          
            "mov %[fd], %%edi;"       
            "mov %[text], %%rsi;"     
            "movq %%rcx, %%rdx;"      
            "syscall;"
            :
            : [fd] "r" (fileDescriptor), [text] "r" (text)
            : "rax", "rdi", "rsi", "rdx", "rcx", "r11", "cc", "memory"
        );
    };

  private:
    int fileDescriptor;
};



int main()
{
  FileManager fileManager("test.txt");
  fileManager.write("a PUSSSYYYYYY\n");

  return 0;
}
