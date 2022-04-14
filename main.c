// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* struct for movie information */
struct movie
{
    char *Title;
    int Year;
    char* Language[5];
    double Rating_value;
    struct movie *next;
};

/* Parse the current line which is space delimited and create a
*  movie struct with the data in this line
*/
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr;

    // The first token is the title
    char *token = strtok_r(currLine, ",", &saveptr);
    currMovie->Title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->Title, token);

    // The next token is the Year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->Year = atoi(token);

    // The next token is the Language
    //remove the title and year part
    token = strtok_r(NULL, ",", &saveptr);
    char*saveptr_2;
    //remove the [] and ;, and append each token to the language array.
    token = strtok_r(token, "[", &saveptr_2);
    token = strtok_r(token, "]", &saveptr_2);
    //printf("token: %s\n", token);
    char*current_language=strtok(token,";");
    
    //the movie has up to 5 languages
    int i;
    for(i=0;i<5;++i)
    {
      // printf("-%s  ", current_language);
      // currMovie->Language[i]=(char*) malloc(21);
      // memset(currMovie->Language[i],'\0',21);
      // strcpy(currMovie->Language[i],current_language);
      //   //currMovie->Language[i]=current_language;
      //   printf("\n%s",currMovie->Language[i]);

      //   //if the pointer is NULL, it will break the loop
      //   if (current_language==NULL)
      //   {
      //      printf("\n\n");
      //       break;
      //   }
      //   current_language=strtok(NULL,";");
      if(current_language==NULL){
        currMovie->Language[i]=NULL;
        break;
      }
      else{
        currMovie->Language[i]=(char*) malloc(21);
        memset(currMovie->Language[i],'\0',21);
        strcpy(currMovie->Language[i],current_language);
        //printf("%s\n",currMovie->Language[i]);
      }
      current_language=strtok(NULL,";");
        
    }
  
    // The last token is the rating value
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->Rating_value = atof(token);

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;
}

/*
* Return a linked list of movies by parsing data from
* each line of the specified file.
*/
struct movie *processFile(char *filePath)
{
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;

    // Read the file line by line
    int i=0;
    int movie_num=0;
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {   //jump the firt line
        if(i==0)
        {
            i+=1;
            continue;
        }
        // Get a new movie node corresponding to the current line
        struct movie *newNode = createMovie(currLine);

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
        movie_num++;
    }
    free(currLine);
    fclose(movieFile);
    printf("Processed file %s and parsed data for %d movies\n", filePath, movie_num);
    return head;
}

/*
* Print data for the given movie
*/
void printMovie(struct movie* amovie){
    printf("%s, %d", amovie->Title, amovie->Year);
    int i;
    for(i=0;i<5;++i)
    {
        char*lan=amovie->Language[i];
        if (lan!=NULL)
        {
            printf(" %s,", lan);
        }
        else
        {
            break;
        }
    }
    printf("%.1f\n",amovie->Rating_value);
}
/*
* Print the linked list of movies
*/
void printMovieList(struct movie *list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}

/*
*   Process the file provided as an argument to the program to
*   create a linked list of student structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o students main.c
*/

//This is the function 1, the user input the year, and will show them the movies released in the year
void FindbyYear(int year,struct movie*list)
{   //this the the initial count of the movie that released in the year
    int foundMovie=0;

    while(list!=NULL)
    {   //if the year of the list is same as the userinput
        //print the title and the count of movies increase
        if(list->Year == year){
            printf("\n%s",list->Title);
            foundMovie++;
        }
        list = list->next;
    }
    //if the count of found movie is 0, it means there is no movie in this year
    if(foundMovie==0)
    {
        printf("\nNo data about movies released in the year %i\n",year);
    }
}

//this is the 2nd function, print the movie that has the highest rate value of each year.
void rateHighestValue(struct movie *list)
{
    struct movie *head = list;
    //a year loop from 1900 to 2021
    for (int year = 1900; year<= 2021; ++year)
    {   //find the hightest value of each year
        struct movie Hight_value;
        //set the initial highestvalue is 0
        Hight_value.Rating_value =0;
         while (list != 0)
         {     //compare the year, and if the year of movies are same, go next step
             if (list->Year == year)
             {  //if the value of the new movie if larger than the saved highest value, use to new value to instead of the old one.
                if (list->Rating_value > Hight_value.Rating_value)
                {
                    Hight_value = *list;
                }
             }
             list = list->next;
         }
         //if the highest value is not 0, means the file has the movie in this year, print the highest rate value and the name.
         if (Hight_value.Rating_value >0)
         {
             printf("%d %.1f %s\n", Hight_value.Year,Hight_value.Rating_value,Hight_value.Title);

         }
         list = head;
    }
    return;
}


//This function is ask user to input a language, and then will output the movies released the language.
void FindbyLanguage(char*language,struct movie*list)
{   //same as FindbyYear
    int foundMovie=0;
    int count=0;
    while(list != NULL) 
    {
        int i;
        for(i=0; i<5; ++i)
        {
          if (list->Language[i]!= NULL){
            //printf("\n%s",list->Language[i]);
            if(strcmp(list->Language[i],language)==0) //use this function to compare the language in the list and the user input
              {
                foundMovie++;
                count=1;
              }

          }
          else{
            break;
          }
            
            
        }

        if(count==1){
          printf("%i, %s\n", list->Year, list->Title);
          count=0;
        }
        //printf("\n%i, %s", list->Year, list->Title);
        list=list->next;
    }
    if(foundMovie == 0)
        printf("\nNo data about movies released in %s\n", language);
}

//function to tell the rule of this program, and get the user's choice
int intro()
{
    int user_input;
    printf("\n1.Show movies released in the specified year.\n"
        "2.Show highest rated movie for each year.\n"
        "3.Show the title and year of release of all movies in a specific language.\n"
        "4.Exit from the program.\n");
    printf("\nEnter a choice from 1 to 4: ");
    scanf("%i", &user_input);
    return user_input;
}

int main(int argc, char *argv[])
{
    int user_choice;
    //wrong path
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./students student_info1.txt\n");
        return EXIT_FAILURE;
    }
    struct movie *list = processFile(argv[1]);
    int user_input;
    //if the user's input is not 4, it will let the user do choice again and again, if the input is 4, the program will Exit
    while (user_input!=4)
    {
        user_input=intro();//get the user's input
        if(user_input==1)//if 1, input the year and go to FindbyYear function
        {
            int year;
            printf("Enter the year for which you want to see movies: ");
            scanf("%d", &year);
            FindbyYear(year,list);
        }
        else if (user_input==2)//if 2, go to rateHighestValue function
        {
            rateHighestValue(list);
        }
        else if (user_input==3)//if 3, input the language and go to FindbyLanguage function
        {
            char* language = malloc(sizeof(char) * 21);
            printf("Enter the language for which you want to see movies: ");
            scanf("%s", language);
            FindbyLanguage(language,list);
            free(language);
        }
        else if (1>user_input||4<user_input)//if input is not 1,2,3,4, ask the user try again
        {
            printf("You entered an incorrect choice. Try again.");
        }

    }
    return EXIT_SUCCESS;//if input is 4, exit
    
}

