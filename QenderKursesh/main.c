#include <stdio.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define maxStudentsPerKurs 6
#define numriKurseve 100
#define maxInstruktor 10
#define maxStudents 100
#define FILE_STUDENT "student.dat"
#define FILE_INSTRUKTOR "instruktor.dat"
#define FILE_KURS "kurs.dat"

const char* kategoria1 = "Programim";
const char* kategoria2 = "Robotike";
const char* kategoria3 = "Graphic Design";

struct Student {
    int id;
    char emri[10];
    char mbiemri[10];
    char ditelindja[11];
};

struct Instruktor {
    int id;
    char emri[10];
    char mbiemri[10];
    char ditelindja[11];
    char kategoria[20];
};

struct Kursi {
    int id;
    char diteNisja[10];
    char ditet[10];
    char orari[20];
    char kategoria[20];
    struct Instruktor instruktor;
    struct Student students[maxStudentsPerKurs];
    int numStudents;
};

void printoMenu() {
    printf("\nZgjidhni 1 per te regjistruar kurs\n");
    printf("Zgjidhni 2 per te regjistruar instruktor\n");
    printf("Zgjidhni 3 per te regjistruar student\n");
    printf("Zgjidhni 4 per te fshire kurs\n");
    printf("Zgjidhni 5 per te fshire instruktor\n");
    printf("Zgjidhni 6 per te fshire student\n");
    printf("Zgjidhni 7 per te listu kurs\n");
    printf("Zgjidhni 8 per te listu studentin\n");
    printf("Zgjidhni 9 per te listu instruktoren\n");
    printf("Zgjidhni 10 per te listu studentet ne kurs\n");
    printf("Zgjidhni 11 per te listu kursit e instruktoreve\n");
    printf("Zgjidhni 12 per te listu studentet e instruktoreve\n");
    printf("Zgjidhni 13 per te mbyllur programin\n");
}

void rregjistroInstruktor(struct Instruktor* instruktor, int* index) {
    printf("Vendosni ID e Instruktorit: ");
    scanf("%d", &instruktor->id);
    printf("Vendosni Emrin e Instruktorit: ");
    scanf("%s", instruktor->emri);
    printf("Vendosni Mbiemrin e Instruktorit: ");
    scanf("%s", instruktor->mbiemri);
    printf("Vendosni ditëlindjen e Instruktorit (DD-MM-YYYY): ");
    scanf("%s", instruktor->ditelindja);

    int numriKursevePerInstruktor;
    printf("Zgjidhni numrin e kursit që jep instruktori:\n"
           "1-Robotike; 2-Programim; 3-Graphic Design: ");
    scanf("%d", &numriKursevePerInstruktor);

    switch (numriKursevePerInstruktor) {
        case 1:
            strcpy(instruktor->kategoria, kategoria1);
            break;
        case 2:
            strcpy(instruktor->kategoria, kategoria2);
            break;
        case 3:
            strcpy(instruktor->kategoria, kategoria3);
            break;
        default:
            printf("Kategori e pavlefshme\n");
            break;
    }

        FILE *file;
        file = fopen(FILE_INSTRUKTOR, "ab");
        if (file == NULL) {
            printf("Gabim në hapjen e file %s.\n", FILE_INSTRUKTOR);
            return;
        }
        fwrite(&instruktor, sizeof(struct Instruktor), 1, file);
        fclose(file);
    printf("Instruktori u rregjistrua me sukses.\n");
    (*index)++;
}

struct Instruktor* gjejInstruktor(struct Instruktor instruktor[], int id) {
    for (int i = 0; i < maxInstruktor; i++) {
        if (instruktor[i].id == id) {
            return &instruktor[i];
        }
    }
    return NULL;
}

void rregjistroKurs(struct Kursi* kursi, struct Instruktor* instruktor) {
    printf("Vendosni ID e kursit: ");
    scanf("%d", &kursi->id);

    printf("Vendosni ditet e kursit (Day1-Day2): ");
    scanf("%s", kursi->ditet);

    int zgjedhja;
    printf("Zgjidhni kategorin e kursit:\n"
           "1-Robotike; 2-Programim; 3-Graphic Design: ");
    scanf("%d", &zgjedhja);

    switch (zgjedhja) {
        case 1:
            strcpy(kursi->kategoria, kategoria1);
            break;
        case 2:
            strcpy(kursi->kategoria, kategoria2);
            break;
        case 3:
            strcpy(kursi->kategoria, kategoria3);
            break;
        default:
            printf("Kursi mund te jete vetem i kategorise : Programim, Robotike, Graphic Design\n");
            return;
    }

    // Zgjedhja e instruktorit
    do {
        printf("Zgjidhni nje ID instruktori nga instruktoret e rregjistruar: ");
        int instruktoriId;
        scanf("%d", &instruktoriId);

        struct Instruktor* gjeturInstruktor = gjejInstruktor(instruktor, instruktoriId);

        if (gjeturInstruktor == NULL) {
            printf("Instruktori me ID %d nuk u gjet. Ju lutem provoni përsëri.\n", instruktoriId);
        } else if (strcmp(gjeturInstruktor->kategoria, kursi->kategoria) != 0) {
            printf("Ky instruktor nuk mund te zhvilloje këtë kurs, ju lutem zgjidhni një instruktor tjetër!\n");
        } else {
            kursi->instruktor = *gjeturInstruktor;
            printf("Instruktori u regjistrua me sukses ne kurs.\n");
            break;
        }
    } while (1);

    printf("Zgjidhni orarin: ");
    scanf("%s", kursi->orari);
    //Shtimi ne file
    FILE *file;
    file = fopen(FILE_KURS, "ab");
    if (file == NULL) {
        printf("Gabim në hapjen e file %s.\n", FILE_KURS);
        return;
    }
    fwrite(&instruktor, sizeof(struct Kursi), 1, file);
    fclose(file);
}

void rregjistroStudent(struct Student* student) {
    printf("Vendosni ID e Studentit: ");
    scanf("%d", &student->id);
    printf("Vendosni Emrin e Studentit: ");
    scanf("%s", student->emri);
    printf("Vendosni Mbiemrin e Studentit: ");
    scanf("%s", student->mbiemri);
    printf("Vendosni ditelindjen e Studentit (DD-MM-YYYY): ");
    scanf("%s", student->ditelindja);

    int dita, muaj, viti;
    sscanf(student->ditelindja, "%d-%d-%d", &dita, &muaj, &viti);

    time_t t = time(NULL);
    struct tm* current_time = localtime(&t);
    int ditaTani = current_time->tm_mday;
    int muajiTani = current_time->tm_mon + 1;
    int vitiTani = current_time->tm_year + 1900;

    int mosha;
    if (muajiTani >= muaj && ditaTani >= dita) {
        mosha = vitiTani - viti;
    } else {
        mosha = vitiTani - viti - 1;
    }

    if (mosha < 5 || mosha > 14) {
        printf("Ky student nuk mund te regjistrohet pasi nuk ka moshen e lejuar per kurset!\n");
    }else{
        FILE *file;
        file = fopen(FILE_STUDENT, "ab");
        if (file == NULL) {
            printf("Gabim në hapjen e file %s.\n", FILE_STUDENT);
            return;
        }
        fwrite(&student, sizeof(struct Student), 1, file);
        fclose(file);
        printf("U rregjistrua me SUKSES!");
    }
}

void fshiInstruktor(struct Instruktor instruktor[maxInstruktor], int id) {
    int indeksi = -1;

    for (int i = 0; i < maxInstruktor; i++) {
        if (instruktor[i].id == id) {
            indeksi = i;
            break;
        }
    }

    if (indeksi == -1) {
        printf("Nuk u gjet instruktori me ID %d.\n", id);
        return;
    }

    if (instruktor[indeksi].kategoria != NULL) {
        printf("Instruktori nuk mund te fshihet sepse ka kurs.\n");
        return;
    }

    for (int i = indeksi; i < maxInstruktor - 1; i++) {
        instruktor[i] = instruktor[i + 1];
    }

    printf("Instruktori u fshi me sukses.\n");
}

void fshiKurs(struct Kursi kursi[numriKurseve], int id) {
    int indeksi = -1;

    for (int i = 0; i < numriKurseve; i++) {
        if (kursi[i].id == id) {
            indeksi = i;
            break;
        }
    }

    if (indeksi == -1) {
        printf("Nuk u gjet kursi me ID %d.\n", id);
        return;
    }

    for (int i = indeksi; i < numriKurseve - 1; i++) {
        kursi[i] = kursi[i + 1];
    }

    printf("Kursi u fshi me sukses.\n");
}

void fshiStudent(struct Kursi kursi[numriKurseve], int id) {
    int indeksi = -1;

    for (int i = 0; i < numriKurseve; i++) {
        for (int j = 0; j < maxStudentsPerKurs; j++) {
            if (kursi[i].students[j].id == id) {
                indeksi = i;
                break;
            }
        }
        if (indeksi != -1) {
            break;
        }
    }

    if (indeksi == -1) {
        printf("Nuk u gjet studenti me ID %d.\n", id);
        return;
    }

    for (int j = 0; j < maxStudentsPerKurs; j++) {
        if (kursi[indeksi].students[j].id == id) {
            for (int k = j; k < maxStudentsPerKurs - 1; k++) {
                kursi[indeksi].students[k] = kursi[indeksi].students[k + 1];
            }
            kursi[indeksi].numStudents--;
            printf("Studenti u fshi me sukses nga kursi.\n");
            break;
        }
    }
}

void listoKurs(struct Kursi kursi[numriKurseve]) {
    printf("Listimi i kursit: \n");
    for (int i = 0; i < numriKurseve; i++) {
        printf("ID: %d, Ditet: %s, Orari: %s, Kategoria: %s, Instruktori: %s %s\n",
               kursi[i].id, kursi[i].ditet, kursi[i].orari, kursi[i].kategoria, kursi[i].instruktor.emri, kursi[i].instruktor.mbiemri);
    }
}

void listoStudentin(struct Student student[maxStudents]) {
    printf("Listimi i studentit: \n");
    for (int i = 0; i < maxStudents; i++) {
        printf("ID: %d, Emri: %s, Mbiemri: %s, Ditelindja: %s\n",
               student[i].id, student[i].emri, student[i].mbiemri, student[i].ditelindja);
    }
}

void listoInstruktoren(struct Instruktor instruktor[maxInstruktor]) {
    printf("Listimi i instruktorit: \n");
    for (int i = 0; i < maxInstruktor; i++) {
        if (instruktor[i].id != -1) {
            printf("ID: %d, Emri: %s, Mbiemri: %s, Ditelindja: %s, Kategoria: %s\n",
                   instruktor[i].id, instruktor[i].emri, instruktor[i].mbiemri, instruktor[i].ditelindja, instruktor[i].kategoria);
        }
    }
}

void listoStudentetNeKurs(struct Kursi kursi[numriKurseve], int id) {
    int indeksi = -1;

    for (int i = 0; i < numriKurseve; i++) {
        if (kursi[i].id == id) {
            indeksi = i;
            break;
        }
    }

    if (indeksi == -1) {
        printf("Nuk u gjet kursi me ID %d.\n", id);
        return;
    }

    printf("Studentet ne kursin me ID %d:\n", id);
    for (int j = 0; j < maxStudentsPerKurs; j++) {
        if (kursi[indeksi].students[j].id != -1) {
            printf("ID: %d, Emri: %s, Mbiemri: %s, Ditelindja: %s\n",
                   kursi[indeksi].students[j].id, kursi[indeksi].students[j].emri, kursi[indeksi].students[j].mbiemri, kursi[indeksi].students[j].ditelindja);
        }
    }
}

void listoKursitEInstruktoreve(struct Kursi kursi[numriKurseve], int id) {
    int indeksi = -1;

    for (int i = 0; i < numriKurseve; i++) {
        if (kursi[i].instruktor.id == id) {
            indeksi = i;
            break;
        }
    }

    if (indeksi == -1) {
        printf("Nuk u gjet instruktori me ID %d.\n", id);
        return;
    }

    printf("Kursit e instruktorit me ID %d:\n", id);
    for (int i = 0; i < numriKurseve; i++) {
        if (kursi[i].instruktor.id == id) {
            printf("ID: %d, Ditet: %s, Orari: %s, Kategoria: %s\n",
                   kursi[i].id, kursi[i].ditet, kursi[i].orari, kursi[i].kategoria);
        }
    }
}

void listoStudentetEInstruktoreve(struct Kursi kursi[numriKurseve], int id) {
    int found = 0;

    printf("Studentet e instruktorit me ID %d:\n", id);
    for (int i = 0; i < numriKurseve; i++) {
        if (kursi[i].instruktor.id == id) {
            for (int j = 0; j < maxStudentsPerKurs; j++) {
                if (kursi[i].students[j].id != -1) {
                    printf("ID: %d, Emri: %s, Mbiemri: %s, Ditelindja: %s, Kursi ID: %d\n",
                           kursi[i].students[j].id, kursi[i].students[j].emri, kursi[i].students[j].mbiemri, kursi[i].students[j].ditelindja, kursi[i].id);
                    found = 1;
                }
            }
        }
    }

    if (!found) {
        printf("Nuk u gjet asnje student per instruktorin me ID %d.\n", id);
    }
}

int main() {
    struct Kursi kursi[numriKurseve];
    struct Student student[maxStudents];
    struct Instruktor instruktor[maxInstruktor];

    for (int i = 0; i < maxInstruktor; i++) {
        instruktor[i].id = -1;
    }

    for (int i = 0; i < numriKurseve; i++) {
        kursi[i].id = -1;
    }

    for (int i = 0; i < maxStudents; i++) {
        student[i].id = -1;
    }

    int countInstruktor = 0;
    int countKurs = 0;
    int countStudent = 0;

    while (1) {
        int zgjedhja;
        printoMenu();
        printf("Zgjidhni: ");
        scanf("%d", &zgjedhja);

        switch (zgjedhja) {
            case 1:
                if (countKurs < numriKurseve) {
                    rregjistroKurs(&kursi[countKurs], instruktor);
                    countKurs++;
                } else {
                    printf("Kapaciteti i kursit është i mbushur, nuk mund te rregjistrohen me kurs.\n");
                }
                break;
            case 2:
                if (countInstruktor < maxInstruktor) {
                    rregjistroInstruktor(&instruktor[countInstruktor], &countInstruktor);
                } else {
                    printf("Kapaciteti i instruktorit është i mbushur, nuk mund të rregjistrohen më shumë instruktorë.\n");
                }
                break;
            case 3:
                if (countStudent < maxStudents) {
                    rregjistroStudent(&student[countStudent]);
                    countStudent++;
                } else {
                    printf("Kapaciteti i studentit është i mbushur, nuk mund te rregjistrohen me student.\n");
                }
                break;
            case 4:
                if (countKurs > 0) {
                    fshiKurs(kursi, maxStudents);
                    countKurs--;
                } else {
                    printf("Nuk ka kursa per te fshire.\n");
                }
                break;
            case 5:
                if (countInstruktor > 0) {
                int id;
                    printf("Vendos ID e Instruktorit qe doni te fshini.");
                    scanf("%d", &id);
                    fshiInstruktor(instruktor,id);
                    countInstruktor--;
                } else {
                    printf("Nuk ka instruktor per te fshire.\n");
                }
                break;
            case 6:
                if (countStudent > 0) {
                    int id;
                    printf("vendos id e Studentit: ");
                    scanf("%d",&id);
                    fshiStudent(kursi, id);
                    countStudent--;
                } else {
                    printf("Nuk ka studente per te fshire.\n");
                }
                break;
            case 7:
                if (countKurs > 0) {
                    listoKurs(kursi);
                } else {
                    printf("Nuk ka kursa per tu shfaqur.\n");
                }
                break;
            case 8:
                if (countInstruktor > 0) {
                    listoInstruktoren(instruktor);
                } else {
                    printf("Nuk ka instruktor per tu shfaqur.\n");
                }
                break;
            case 9:
                if (countStudent > 0) {
                    listoStudentin(student);
                } else {
                    printf("Nuk ka studente per tu shfaqur.\n");
                }
                break;
            case 10:
                if (countKurs > 0) {
                    int idKursi;
                    printf("Shkruani ID e kursit per te shfaqur studentet: ");
                    scanf("%d", &idKursi);
                    listoStudentetNeKurs(kursi, idKursi);
                } else {
                    printf("Nuk ka kursa per te shfaqur studentet.\n");
                }
                break;
            case 11:
                if (countInstruktor > 0) {
                    int idInstruktori;
                    printf("Shkruani ID e instruktorit per te shfaqur kursin: ");
                    scanf("%d", &idInstruktori);
                    listoKursitEInstruktoreve(kursi, idInstruktori);
                } else {
                    printf("Nuk ka instruktor per te shfaqur kursin.\n");
                }
                break;
            case 12:
                if (countInstruktor > 0) {
                    int idInstruktori;
                    printf("Shkruani ID e instruktorit per te shfaqur studentet: ");
                    scanf("%d", &idInstruktori);
                    listoStudentetEInstruktoreve(kursi, idInstruktori);
                } else {
                    printf("Nuk ka instruktor per te shfaqur studentet.\n");
                }
                break;
            case 13:
                printf("Deshironi te dilni? (po=1/ jo=0): ");
                scanf("%d", &zgjedhja);
                if (zgjedhja == 1) {
                    return 0;
                }
                break;
            default:
                printf("Zgjidhni nje opsion te vlefshem.\n");
                break;
        }
    }

    return 0;
}
