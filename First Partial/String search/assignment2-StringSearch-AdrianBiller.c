#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*Advanced programming
Homework 2 String search
By: Adrian Biller Alcantara A01018940
January 2017
ITESM CSF*/

char *input(int size){
  char input[size]; //the input array of chars depending on wether it is the sentence or word
  fgets(input, size, stdin);//function fgets that obtains from buffer the input written
  input[strlen(input)-1] = '\0';//adding the "end" attribute to the char array for further use
  return (char *)input;
}

int checkSentence(char* sentence, char* word){
  int count = 0;
  int i = 0;
  int j;
  while(sentence[i] != '\0'){ //going through all the sentence until the char indicating the end of the string
    j = 0;
    while(sentence[i] == word[j]){ //constantly checking if a char is similar then it may be the same word
      if(word[j+1] == '\0'){ //if reaching to the end of the word it means that both are the same word
        count++;              //consequently adding to the counter indicating a word has been found
        break;              //reinitializing the word and continuing the sentence to search for more
      }
      i++;
      j++;
    }
    i++;
  }
  return count;// return of the number of words found
}

void stringSearch(){
  char sentence [100]= "null";//initializing the sentence array
  char word [100] = "null";//initializing the word array with 100 if done with 10 then it would not add the correct word
  printf("Please write the sentence that will be analyzed\n");
  strcpy(sentence,input(100)); //string copy function where the input function is copied and assigned to the previous variables
  printf("Write the word that will be checked\n");
  strcpy(word,input(100));//same string copy function
  int numberOfwords = checkSentence(sentence, word); //call on checkSentence function
  printf("Number of times the word is on this sentence: %d\n", numberOfwords);
}

int main(){
  stringSearch();
  return 0;
}
