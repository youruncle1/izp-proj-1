#include <stdio.h>
#include <stdlib.h>

int nacitaj_heslo(char *heslo);                     //nacita heslo zo stdin a ulozi ho do heslo[]
void vypis_heslo(char *heslo);                      //vypise dane heslo do stdout
int arr_length(char *arr);                          //zisti dlzku pola arr[]
int my_strcmp(char *arr1, char *arr2);              //strcmp...           
int is_digit_only(char *arr);                       //zisti, ci sa v retezci nachadzaju iba cisla
int is_in_arr(char *arr, char c);                   //zisti, ci sa char c nachadza v poli arr[]
int has_sequence(char *heslo, int parameter);       //zisti, ci sa v heslo[] nachadza sekvencia znakov dlzky parameter
int has_substring(char *heslo, int parameter);      //zisti, ci sa v heslo[] nachadza podretezec dlzky parameter
int arg_check(int argc, int lvl, int param);        //overi validnost command line argumentov
int lvl_12_check(char *heslo);                      //zisti, ci heslo[] vyhovuje lvl1/lvl2

int nacitaj_heslo(char *heslo)
{
    int character;
    character = fgetc(stdin);

    for (int i = 0; i < 101; i++)
    {
        if (character == '\n' || character == EOF)
        {
            heslo[i] = '\0';
            if (character == EOF)
                return -1;  //cyklus sa dostal po EOF
            return 0;
        }
        heslo[i] = (char)character;
        character = fgetc(stdin);
    }
    return 1;  //cyklus sa nedostal po '\n' alebo eof => zadane heslo bude mat viac ako 100 znakov
   
}

void vypis_heslo(char *heslo)
{
    for (int i = 0; heslo[i] != '\0'; i++)
        fprintf(stdout, "%c", heslo[i]);
    printf("\n");
}

int arr_length(char *arr)
{
    int pocet_znakov = 0;
    while (arr[pocet_znakov] != '\0')
        pocet_znakov++;
    return pocet_znakov;
}

int my_strcmp(char *arr1, char *arr2)
{
    if (arr_length(arr1) != arr_length(arr2))       //ak sa dlzka retazcov nerovna <=> retazce sa nerovnaju
        return 0;
    for (int i = 0; arr1[i] != '\0'; i++)
    {
        if (arr1[i] != arr2[i])
            return 0;
    }
    return 1;

}

int is_digit_only(char *arr)
{
    for (int i = 0; arr[i] != '\0'; i++)
    {
        if (!(arr[i] >= '0' && arr[i] <= '9'))
            return 0;
    }
    return 1;
}

int is_in_arr(char *arr, char c)
{
    for (int i = 0; arr[i] != '\0'; i++)
    {
        if (arr[i] == c)
            return 1;
    }
    return 0;
}

int has_sequence(char *heslo, int parameter)
{
    // zoberem index1 v hesle(1. cycle), porovnavam s dalsim(2. cycle), ak !=, idem na dalsi index a porovnavam, ak sedi, pripocitam do pocitadla
    // ak sa najde sekvencia dlzky parameter || prejde od indx1 po koniec hesla bez najdenia, 2. cyklus skonci a pozrie sa na podmienku, ci counter == parameter.
    int counter = 1;
    
    for (int i = 0; heslo[i] != '\0'; i++)
    {
        if (counter == parameter)
            return 1;

        for (int n = 1; n < parameter; n++)
        {
            if (heslo[i] == heslo[i+n])
                counter += 1;
            else
                counter = 1;
            
        }
    }
    return 0;
}

int has_substring(char *heslo, int parameter)
{
    //zoberem index i z hesla, index i+1, ak narazim na rovnaky char, pocitam postupnost charakterov(pridavam i a j ak sa rovnaju)
    //ak count == parameter, nasiel v hesle dalsi rovnaky substring dlzky aspon parameter
    int char_count;
    int index;

    for (int i = 0;  heslo[i+parameter] != '\0'; i++)
    {
        char_count = 0;
        index = i;
        for (int j = i+1; heslo[j] != '\0'; j++)
        {
            //printf("counter : %d\nindex = %d\ni = %d\nj = %d\n", char_count, index, i, j);
            if (heslo[index] == heslo[j])
            {
                char_count++;
                if (heslo[j+1] == heslo[j] && index < 1)
                {
                    /*ak ma hladany substring prvy index v sekvencii >1
                      - bez tejto podmienky by heslo abcaabc nenasiel rovnaky substr
                      - toto plati iba ak nastane sekvencia pri porovnavani prveho indexu(preto index < 1),
                      - inak by nasiel substr aj pri abcabbc, co je v tomto pripade falsepositive
                      - cyklus preskoci j o dlzku sekvencie(ak je sekvencia >= parameter --> to vyriesi lvl 3)*/
                    int skip_index = 0;
                    for (int seq_dlzka = 1; heslo[j] == heslo[j+seq_dlzka]; seq_dlzka++)
                        skip_index++;
                    j += skip_index;
                }
                index++;
            }
            else if (heslo[index] != heslo[j])
            {
                index = i;
                char_count = 0;
            }
            if (char_count == parameter)
            {
                return 1;
            }
            
        }
        
    }   
    return 0;
}

int arg_check(int argc, int lvl, int param)
{
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "Error: invalid arg count(%d)!", argc);
        return -1;
    }
    if (lvl == 0 || param == 0)
    {
        fprintf(stderr, "Error: invalid arguments!");
        return -2;
    }
    if (lvl < 1 || lvl > 4)
    {
        fprintf(stderr, "Error: Level argument not in [1,4] interval!");
        return -14;
    }
    return 0;
}

int lvl_12_check(char *heslo)
{
    //funckia vrati -1 ak lvl1 neprejde, inak vrati pocet skupin ktore splna heslo v lvl2

    int male_pismena = 0;
    int velke_pismena = 0;
    int cisla = 0;
    int spec = 0;
    
    
    for (int i = 0; heslo[i] != '\0'; i++)
    {
        if (heslo[i] >= 'A' && heslo[i] <= 'Z')
            velke_pismena = 1;

        else if (heslo [i] >= 'a' && heslo[i] <= 'z')
            male_pismena = 1;

        else if(heslo[i] >= '0' && heslo[i] <= '9')
            cisla = 1;

        else if ((heslo[i] >= ' ' && heslo[i] <= '/') || 
                (heslo[i] >= ':' && heslo[i] <= '@') || 
                (heslo[i] >= '['  && heslo[i] <= '`')||
                (heslo[i] >= '{'  && heslo[i] <= '~'))
            spec = 1;
    }


    int parameter = male_pismena + velke_pismena + cisla + spec;
    /*
    * -1 nesplnilo ani jeden level
    2 male a velke pismena // lvl1
    3 lvl1, jedno z cisla/spec
    4 lvl1,cisla, specialne znaky
    */
    if (male_pismena && velke_pismena)
    {
        return parameter;
    } else {
       return -1;
    }
}


int main(int argc, char *argv[])
{          
    int error_num;                          //return value na arg_check()                  
    long int lvl = 0, param = 0;
    int stats_check = 0;                    //1 -> vypis statistiku, 0 -> nevypisuj statistiku

    if (argc >= 3) 
    {
        //inicializovat lvl a param mozem az ak overim, ze sa v argv[] nachadzaju
        lvl = strtol(argv[1], NULL, 10);
        param = strtol(argv[2], NULL, 10);
        
        if (is_digit_only(argv[1]) == 0 || is_digit_only(argv[2]) == 0) //ak lvl/param obsahuje pismena => invalid input
        {
            fprintf(stderr, "Error: invalid arguments!");
            return -2;
        }
        
        if (argc == 4) //ak je argc 4, argv[3] musi byt --stats inak zle zadany argument
        {
            if (my_strcmp(argv[3], "--stats") == 0)
            {
                fprintf(stderr, "Error: invalid argument 4!(has to be --stats or none)");
                return -5;
            }
            else
                stats_check = 1;
        }
    }
    
    if ((error_num = arg_check(argc, lvl ,param)) != 0) 
        return error_num;

    char heslo[101];                        //pole na ulozenie hesla
    int lvl_12_param = param;               //upresnenie projektu k lvl2
    int ret;                                //return value z funkcie nacitaj_heslo()
    
    //Statistika
    char nchars[128];                       //pole na ukladanie roznych znakov
    int nchars_index = 0;                   //na iterovanie nchars
    int min_length = 0;                     //minimalna dlzka hesla
    int length_sum = 0;                     //suma dlzek hesiel
    int password_count = 0;                 //pocet validnych hesiel
    long double pw_avg = 0.0;               //avg dlzky hesiel
    
    while (1)
    {
        ret = nacitaj_heslo(heslo);
        
        if (ret == 1)
        {
            fprintf(stderr, "Error: Password length exceeds 100 characters! Invalid data.");
            return -101;
        }

        if (param > 4)
            lvl_12_param = 4; // upresnenie projektu v lvl2 ak x>pocetskupin tak x = 4(vsechny)
        
        if (lvl == 2 || lvl == 1)
        {
            if (lvl == 1)
                lvl_12_param = 2; //na prejdenie lvl_12_check s lvl1 je potrebne param max 2(moze splnat viac skupin a lvl_12_check vrati pocet skupin)
            if (lvl_12_check(heslo) >= lvl_12_param) {
                vypis_heslo(heslo);
            }
        }

        else if (lvl == 3)
        {
            if (lvl_12_check(heslo) >= lvl_12_param)
            {
                if (has_sequence(heslo,param) == 0)
                {
                    vypis_heslo(heslo);
                }
            }
        }

        else if (lvl == 4)
        {
            if (lvl_12_check(heslo) >= lvl_12_param)
            {
                if (has_sequence(heslo,param) == 0)
                {
                    if (has_substring(heslo, param) == 0)
                    {
                        vypis_heslo(heslo);
                    }
                }
            }
        }

        if(ret == -1) // ak nacitaj_heslo() najde EOF
            break;

        //statistika
        password_count++;
        length_sum += arr_length(heslo);

        if (password_count == 1)
            min_length = arr_length(heslo);
        min_length = arr_length(heslo) < min_length ? arr_length(heslo) : min_length;

        for (int i = 0; heslo[i] != '\0'; i++)
        {
            if (is_in_arr(nchars, heslo[i]) == 0)
            {
                nchars[nchars_index] = heslo[i];
                nchars_index++;
                nchars[nchars_index] = '\0';
            }
        }
    }

    //statistika
    if (stats_check == 1)
    {   
        fprintf(stdout, "Statistika:\n");
        fprintf(stdout, "Ruznych znaku: %d\n", arr_length(nchars));
        fprintf(stdout, "Minimalni delka: %d\n", min_length);
        if (password_count != 0)
        {
            pw_avg = (long double)length_sum/password_count;
            fprintf(stdout,"Prumerna delka: %.1Lf\n", pw_avg);
        }
        else
            fprintf(stdout, "Prumerna delka: 0\n");  
         
    }

    return 0;
    
}