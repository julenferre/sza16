#define MAX_BUF 1024
#define SERVER "localhost"
#define PORT 6012

#define COM_ERRG  0
#define COM_USER	1
#define COM_PASS	2
#define COM_ENTR  3
#define COM_LIST	4
#define COM_PLAY  5
#define COM_SOLV  6
#define COM_STOP  7
#define COM_RANK  8
#define COM_RNKG  9
#define COM_EXIT	10


char * KOMANDOAK[] = {"ERRG","USER","PASS","ENTR","LIST","PLAY","SOLV","STOP","RANK","RNKG","EXIT",NULL};
char * ER_MEZUAK[] =
{
	"Dena ondo. Errorerik ez.\n",
	"Komando ezezaguna edo ustegabekoa.\n",
	"Erabiltzaile ezezaguna.\n",
	"Pasahitz okerra.\n",
	"Ezin izan da lista erakutsi.\n",
  "Errore bat suertatu da jokaldian.\n",
  "Ezin izan da jokua amaitu.\n",
  "Ezin izan da puntuazio-lista erakutsi.\n",
  "Erabiltzailea existitzen da.\n"
};

void jokuaItxi(int sock);
int hasierakoMenua();
int menuNagusia();
