#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

struct Employee {
    char id[10];
    char name[50];
    char password[50];
    int is_employed;
};
void write_employee_records(int fd) {
    struct Employee emp1 = {"E1001", "Alice", "pass1", 1};
    struct Employee emp2 = {"E1002", "Bob", "pass2", 1};
    
    write(fd, &emp1, sizeof(emp1));  // Write first employee
    write(fd, &emp2, sizeof(emp2));  // Write second employee
}

int main() {
    int db_fd = open("employees.dat", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (db_fd == -1) {
        perror("Error opening file");
        return 1;
    }
    
    write_employee_records(db_fd);
    close(db_fd);
    return 0;
}
