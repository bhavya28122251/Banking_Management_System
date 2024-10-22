#include "header_file.h"

bool add_feedback(int cd){
	struct Feedback feed;

        char buf_name[]="-----Give Feedback-----\nEnter your ID:";
        write(cd,buf_name,sizeof(buf_name));

        int bytes_id=read(cd,feed.id,sizeof(feed.id));
        if(bytes_id<=0){
                write(cd,"Error in receiving data from client\n",strlen("Error in receiving data from client\n"));
                return false;
        }
        feed.id[bytes_id]='\0';
        if(feed.id[bytes_id-1]=='\n'){
                feed.id[bytes_id-1]='\0';
        }

	char buf_feed[]="Your Feedback:";
        write(cd,buf_feed,sizeof(buf_feed));

        int bytes_feed=read(cd,feed.feedback,sizeof(feed.feedback));
        if(bytes_feed<=0){
                write(cd,"Error in receiving data from client\n",strlen("Error in receiving data from client\n"));
                return false;
        }
        feed.feedback[bytes_feed]='\0';
        if(feed.feedback[bytes_feed-1]=='\n'){
                feed.feedback[bytes_feed-1]='\0';
        }

        FILE *file=fopen("feedback_db.txt","a");
        if(file!=NULL){
                fprintf(file,"%s,%s\n",feed.id,feed.feedback);
                fclose(file);
                printf("true\n");
                fflush(stdout);
                return true;
        }
        else{
	perror("Error in opening file");
        printf("false\n");
        fflush(stdout);
        return false;
	}
}


bool review_feedback(int cd){
	char cust_id[100];

        const char* prompt1="-----Review Feedback-----\nEnter ID of the Customer whose feedback you want to review:";
        if(( write(cd,prompt1,strlen(prompt1)))==-1){
                perror("Error");
        }

        ssize_t data_read = read(cd,cust_id,sizeof(cust_id));

        if (data_read <= 0)
        {
                close(cd);
                exit(1);
        }

        if(cust_id[data_read-1]=='\n')
                cust_id[data_read-1]='\0';
        else    
                cust_id[data_read]='\0';

        int db_fd = open("feedback_db.txt", O_RDONLY);
        if (db_fd == -1) {
                perror("Error in opening the database file");
                return false;
        }

        char line[300];
        struct Feedback temp;


         char buffer;
        int line_index = 0;

        while (read(db_fd, &buffer, 1) > 0) {
        if (buffer != '\n') {
            line[line_index++] = buffer;
        }
         else {
                line[line_index] = '\0';
                line_index = 0;

		sscanf(line, "%[^,],%[^,]", temp.id, temp.feedback);

		if(strcmp(temp.id,cust_id)==0){

			char buff[256];
			int len = sprintf(buff, "Feedback: %s\n", temp.feedback); 
			write(cd, buff, len);

			printf("feedback\n");
                	return true;

            }
        }}
        write(cd,"Customer not Found",strlen("Customer not Found"));
        return false;
}
