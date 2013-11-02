/*
  Author: Vadym Yatsyuk
  Programma che utilizzando i file gestisca un'agenda,
  Si puo' aggiungere eventi, cancellare eventi(tutti insieme o un solo evento),
  si puo' visualizzare tutti eventi o visualizzare utilizzando ricerca la descrizione del evento
  se quello esiste e poi si puo' cancellare evento;
 */

#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

void stampaCalendar();
void inserimentoCalendar(bool scelta);
void menu(bool &exit);
void cerca();
int countEvent();

struct day
{
    int giorno;
    int mese;
    int anno;
    char descrizione[20];
}day;

//crea il file;
ofstream file("calendar.txt");

int main(int argc, char *argv[])
{
    bool exit = true;
    while(exit)
    {
        menu(exit);
    }
    file.close();
    if( remove( "calendar.txt" ) != 0 )
    {
        perror( "Error deleting file" );
    }
    else
    {
        puts( "File successfully deleted" );
    }
    return 0;
}

void menu(bool &exit)
{
    // funzione MENU, con quale inizia programma e si vede dopo ogni procedura
    // si stampa le possibilita' del utente che puo' fare in questa programma
    // e si prende in input la scelta del utente e si passa alla procedura scelta dal utente.
    int scelta;
    bool decisione = true;
    do
    {
        system("CLS");
        cout << countEvent() << " Eventi\n\n";
        cout <<"Premi [1] per inserire"<<endl;
        cout <<"Premi [2] per visualizzare tutti eventi"<<endl;
        cout <<"Premi [3] per cercare un evento\n\tcancellare,modificare"<<endl;
        cout <<"Premi [4] per cancellare tutto"<<endl;
        cout <<"Premi [5] per uscire"<<endl;
        cin >> scelta;
    }while(scelta < 1 || scelta > 5);
    switch(scelta)
    {
    case 1:
        inserimentoCalendar(decisione);
        break;
    case 2:
        stampaCalendar();
        break;
    case 3:
        cerca();
        break;
    case 4:
        file.close();
        remove("calendar.txt");
        file.open("calendar.txt");
        break;
    case 5:
        exit = false;
        break;
    }
}

void inserimentoCalendar(bool scelta)
{
    // bool scelta serve per il cancellamento e ricerca;
    // funzione che prende in input data e descrizione del evento e secondo del parametro della
    // variabile 'scelta' fa inserire data dentro file o solo dentro la struttura che
    // sara utile per la funzione cerca();
    cout << "Giorno: ";
    cin >> day.giorno;
    cout << "\nMese(in cifre): ";
    cin >> day.mese;
    cout << "\nAnno: ";
    cin >> day.anno;
    if(scelta)
    {
        cout << "\nDescrizione(max20char): ";
        cin.ignore();
        cin.getline(day.descrizione,20);
        file.seekp(0,ios_base::end); // si fa puntare il puntatore al fine del file;
        file << "Data:" <<endl;
        file << day.giorno << endl;
        file << day.mese << endl;
        file << day.anno << endl;
        file << "Descrizione: ";
        file << day.descrizione << endl;
    }
}

void stampaCalendar()
{
    // Procedura che stampa riga per riga dal file;
    system("CLS");
    ifstream stampafile("calendar.txt");
    if( !stampafile.is_open())
    {
        cout << "Errore di apertura\n";
    }
    else
    {
        bool exit=true;
        while(exit)
        {
            char vet[20];
            stampafile >> vet;
            if(!stampafile.eof())
            {
                cout << vet << endl;
            }
            else
            {
                exit = false;
            }
        }
        stampafile.close();
        system("PAUSE");
    }
}

void cerca()
{
    // procedura con quale utente puo' cercare se esiste o per vedere cosa ha scritto per quel giorno
    // utente inserisce g/m/a ogniuno per riga, e se esiste evento in quel girono si puo' cancellarlo
    // o modificarlo
    ifstream file1("calendar.txt");
    if(!file1.is_open())
    {
        cout << "Errore di apertura" << endl;
    }
    else
    {
        cout <<"Inserire la data per cercarla e vedere la descrizione" << endl;
        inserimentoCalendar(false);
        file1.seekg(0,ios_base::beg); // si fa puntare il puntatore al iniz. del file;
        int scelta(1);  // per cancellamento, 1 - si fa cancellare, 0 - no(in caso se la data non e' stata trovata);
        bool exit=true; // per uscire dal ciclo
        int count(0);   // contatore degli eventi per cancellamento, si usa in modo che si aumenta contatore quando
        //si incontra "Data:" finche non si trova data della ricerca e data del evento equivalenti
        char temp[10];   // \ ;
        char temp2[10];  //  - variabili temporali per copiare la data in un vettore;
        char temp3[10];  // / ;
        char vet[20];
        // si copia la data in un solo vettore
        sprintf(temp2, "%i", day.giorno);
        sprintf(temp3, "%i", day.mese);
        strcat(temp2,temp3);
        sprintf(temp3, "%i", day.anno);
        strcat(temp2,temp3);

        while(exit)
        {
            if(!file1.eof())
            {
                file1 >> vet;
                strcpy(temp,"");
                if(strcmp(vet,"Data:") == 0)
                {
                    count++; // serve per cancellamento
                    // si conta eventi, per arrivare ad un evento da cancellare;
                    for(int i=0;i<3;i++) // si copia ogni data in un vettore per controllarla
                    {
                        file1 >> vet;
                        strcat(temp,vet);
                    }
                    if(strcmp(temp,temp2)==0) // se 2 vettori sono uguali stampa la data e la descrizione del girno
                    {
                        system("CLS");
                        cout << day.giorno << "/" << day.mese << "/" << day.anno << endl;
                        file1 >> vet;
                        do
                        {
                            cout << vet << endl;
                            file1 >> vet;
                        }while(strcmp(vet,"Data:")!=0 && !file1.eof());
                        exit=false;
                    }
                }
            }
            else
            {
                cout << "Data non e' stata ancora segnata." <<endl;
                exit = false;
                scelta = 0; // se la data non e stata trovata, cambia il valore e non permette di cancellare evento
            }
        }
        /* Se la data ricercata e' stata trovata, permette di cancellare evento
           o modificarlo;
           Per cancellare/modificare evento si usa la riga "Data:" e con questa si conta
           ogni evento cioe', per contare eventi basta contare quanto si ripete la riga
           "Data:" - funzione countEvent(), quindi, prima quando si cerca evento e si stampa
           la descrizione in quel momento si lavora anche con contatore per
           cancellamento/modifica - 'count', poi quando si va a cancellare/modificare
           si usa un altro contatore 'i' che anche quello si aumenta quando trova
           la riga "Data:", poi dopo aver aumentato una volta si controlla se sono uguali.
           Per il cancellamento/modifica serve un altro file 'temp.txt', cioe' un file
           dove si riscrive gli Eventi pero' senza quel evento da cancellare o al posto
           di qullo che e' stato modificato si riscriva uno nuovo, e poi si riscrive di
           nuovo tutto il file svuotando file 'calendar.txt' tutto che c'e' dentro 'temp.txt'
           e si cancella 'temp.txt'. Se sono UGUALI 'i' e 'count' quindi si passa senza
           riscrivere niente in 'temp.txt' fino alla riga succesiva cercando la riga con
           "Data:", pero' se e' la modifica, si riscrive prima di "Data:" il nuovo evento,
           e poi si riscrive tutto il resto dentro 'temp.txt'.
           Se sono DIVERSI si aumenta contatore 'i' quando si trova "Data:" finche non
           saranno uguali i due contatori.
        */
        if(scelta == 1)
        {
            do
            {
                cout << "[1] - per cancellare evento\n";
                cout << "[2] - per modificare evento\n";
                cout << "[0] - per ritornare  in menu\n";
                cin >> scelta;
            }while(scelta < 0 || scelta > 2 );
            if(scelta==1 || scelta == 2)
            {
                int i(0); // contatore per cercare quale evento deve cancellare;
                ifstream oldFile("calendar.txt");
                ofstream tempFile("temp.txt");
                while(!oldFile.eof())
                {
                    oldFile >> vet;
                    if(strcmp(vet,"Data:") == 0)
                    {
                        i++;
                        if(i != count) // per aggiungere 'Data:';
                        {
                            tempFile << vet << endl; //contiene 'Data:';
                        }
                        if(i == count) // 2 contatori per controllo di un determinato evento
                        {              // scelto da cancellare da utente
                            do
                            {
                                oldFile >> vet;
                                if(strcmp(vet,"Data:") == 0)
                                {
                                    tempFile << vet << endl; // Per aggiungere "Data:";
                                }
                            }while(strcmp(vet,"Data:") != 0 && !oldFile.eof());
                            if(scelta == 2)
                            {
                                cout << "Inserisci la nuova data\n";
                                cout << "Giorno: ";
                                cin >> day.giorno;
                                cout << "\nMese(in cifre): ";
                                cin >> day.mese;
                                cout << "\nAnno: ";
                                cin >> day.anno;
                                cout << "\nDescrizione(max20char): ";
                                cin.ignore();
                                cin.getline(day.descrizione,20);
                                if(strcmp(vet,"Data:") != 0) // per controllare se c'e'
                                {                            // di gia' la riga 'Data:'
                                    tempFile << "Data:" << endl; // in caso se evento e' ultimo - non c'e'
                                }                               // la riga 'Data:', quindi si aggiunge
                                tempFile << day.giorno << endl;
                                tempFile << day.mese << endl;
                                tempFile << day.anno << endl;
                                tempFile << "Descrizione: ";
                                tempFile << day.descrizione << endl;
                            }
                        }
                    }
                    else
                    {
                        if(!oldFile.eof()) // per non aggiungere 2 volte ultima riga;
                        tempFile << vet << endl;
                    }
                }
                oldFile.close();
                tempFile.close();
                remove( "calendar.txt" ); // si cancella 'calendar.txt' per riscriverlo
                ifstream temp("temp.txt");
                ofstream newFile("calendar.txt");
                exit = true;
                while(exit)
                {
                    temp >> vet;
                    if(!temp.eof())
                    {
                        newFile << vet << endl;
                    }
                    else
                    {
                        exit = false;
                    }
                }
                temp.close();
                remove( "temp.txt" );
                newFile.close();
            }
        }
        system("PAUSE");
    }
}

int countEvent() // Funzione che conta numero di eventi che esistono
{                // si puo' vedere il numero in menu;
    int count(0);
    char vet[20];
    ifstream stampa("calendar.txt");
    while(!stampa.eof())
    {
        stampa >> vet;
        if(strcmp(vet,"Data:")==0)
        {
            count++;
        }
    }
    stampa.close();
    return count;
}
