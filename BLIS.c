#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_ENT 100
#define Max_col 10
#define Max_row 10
#define MAX_STR 50

typedef struct
{
    int column;
    int row;
    float size1;
    float size2;
    int Price;
    char status[MAX_STR];
} Lot_Info;

typedef struct
{
    int column;
    int row;
    char fullname[MAX_STR];
    int BirthMonth;
    int BirthDay;
    int BirthYear;
    int DeathMonth;
    int DeathDay;
    int DeathYear;
    char qoute[MAX_STR];
} Info;

Info Read_Entry(FILE *fileptr);
Lot_Info Lot_details(FILE *lotptr);
void MarkLot(char Lot[][Max_col], Info entry);
void PLot(char Lot[][Max_col]);
void Print_info(int col, int row, Info deads[MAX_ENT], Lot_Info lotdet[MAX_ENT]);
void cMonth(int num_month, char *monthptr);

int main()
{
    int Menu_Option;
    int LvisMenu;  // Variable for Menu
    int index = 0; // Index in the array from row and column
    int Lot_index = 0;
    int again;             // Variable for the loop in Choosing Lot Info
    int row_id, column_id; // Column and Row

    Info deads[MAX_ENT]; // Arrays of Struct, 100
    Lot_Info lotdet[MAX_ENT];
    Info entry;         // For the masterlits file with taken lot
    Lot_Info lot_entry; // For scanning the lot details

    char Lot[Max_row][Max_col]; // 10x10 matrix for Lot
    char colid;                 // Char Version of Column

    // Initialize Lot and the array deads as empty
    memset(deads, 0, sizeof(deads));
    memset(lotdet, 0, sizeof(lotdet));
    memset(Lot, ' ', sizeof(Lot));

    FILE *ifp; // File pointer for input
    FILE *ofp; // File pointer for Output
    FILE *lfp; // File pointer for lot input file

    ifp = fopen("MASTERLIST.txt", "rt");
    ofp = fopen("Copylist.txt", "wt");
    lfp = fopen("Lotfile.txt", "rt");

    if (!ifp || !ofp || !lfp)
    {
        printf("Error opening files.\n");
        return 1;
    }

    while (1)
    {
        printf("\n\t\t\t\t\tWelcome to B.L.I.S.\n");
        printf("\t\t\t\t  Burial Lot Information System\n");

        printf("\n%55s\n", "Choose one of the Options below:");
        printf("%54s\n", "1. New Entry");
        printf("%56s\n", "2. Lot Visuals");
        printf("%65s\n", "3. Package Descriptions");
        printf("%49s\n", "4. Exit");
        printf("Option:");
        scanf("%d", &Menu_Option);

        switch (Menu_Option)
        {
        case 1:
            break;
        case 2: // Lot Visualization
            while (1)
            {

                entry = Read_Entry(ifp);
                lot_entry = Lot_details(lfp);

                if (feof(ifp) && ferror(ifp) || feof(lfp) || ferror(lfp))
                    break;

                if (entry.row >= 1 && entry.row <= Max_row && entry.column >= 0 && entry.column <= Max_col)
                {
                    index = ((entry.row - 1) * Max_row) + entry.column;
                    deads[index] = entry;
                    MarkLot(Lot, entry);
                }

                if (lot_entry.row >= 1 && lot_entry.row <= Max_row && lot_entry.column >= 0 && lot_entry.column <= Max_col)
                {
                    Lot_index = ((lot_entry.row - 1) * Max_row) + lot_entry.column;
                    lotdet[Lot_index] = lot_entry;
                }
            }
            // Display the Lot visuals
            PLot(Lot);
            do
            {
                printf("\n%55s\n", "Choose one of the Options below:");
                printf("%55s\n", "1. Choose Lot");
                printf("%54s\n", "2. Main Menu");
                printf("%49s\n", "3. Exit");
                printf("Option: ");
                scanf("%d", &LvisMenu);

                switch (LvisMenu)
                {
                case 1:
                    do
                    {

                        printf("\nEnter Lot(Column-Row): ");
                        scanf(" %c-%d", &colid, &row_id);

                        column_id = toupper(colid) - 'A';

                        Print_info(column_id, row_id, deads, lotdet);

                        printf("\n%55s\n", "Choose one of the Options below:");
                        printf("%57s\n", "1. Choose Again");
                        printf("%54s\n", "2. Main Menu");
                        printf("%49s\n", "3. Exit");
                        printf("Option: ");
                        scanf("%d", &again);

                        switch (again)
                        {
                        case 1:
                            PLot(Lot);
                            break;
                        case 2:
                            break;
                        case 3:
                            printf("Exiting Program...\n");
                            fclose(ifp);
                            fclose(ofp);
                            exit(0);
                        default:
                            printf("Not an option, Try Again\n");
                            printf("\n");
                            break;
                        }

                    } while (again != 2);
                    break;
                case 2:
                    printf("\e[1;1H\e[2J");
                    break;
                case 3:
                    printf("Exiting Program...\n");
                    fclose(ifp);
                    fclose(ofp);
                    exit(0);
                default:
                    printf("Not an option, Try Again\n");
                    printf("\n");
                    break;
                }

                if (again == 2)
                {
                    printf("\e[1;1H\e[2J");
                    break;
                }

            } while (LvisMenu != 2 && LvisMenu != 3);

            // fseek(ifp, 0, SEEK_SET);
            break;

        case 3: // Package Descriptions
            printf("\t\tA: Normal\n");
            printf("\t\t\tThis package includes the burial plot itself, the\n\t\t\topening and closing of the grave and a grave care\n\t\t\tof once every 6 months.\n");
            printf("\t\tB: Premium\n");
            printf("\t\t\tThis package is one level above the Normal package.\n\t\t\tThis package offers a more enhanced service  than\n\t\t\tthe previous level such as additional customization\n\t\t\toption, upgraded materials for the headstone or marker\n\t\t\tand a more frequent grave care of once every 3 months.\n");
            printf("\t\tC: Silver\n");
            printf("\t\t\tIn addition to the features of the premium package,\n\t\t\tthe Silver package may include amenities like occasional\n\t\t\tfloral arrangements, access to support services for the\n\t\t\tbereaved, a selection of standard markers or headstones\n\t\t\twith basic engraving options, priority scheduling for\n\t\t\tcemetery visits or special events, ensuring ease of\n\t\t\taccess and convenience for family members and visitors.\n");
            printf("\t\tD: Gold:\n");
            printf("\t\t\tThe Gold package includes all the benefits of the Silver\n\t\t\tpackage and offers the additional feature of a mausoleum\n\t\t\tfor above-ground interment. This may include a designated\n\t\t\tspace within the mausoleum, personalized inscriptions,\n\t\t\tongoing maintenance of the structure and exclusive access\n\t\t\tto certain areas or facilities within the cemetery grounds\n\t\t\tfor private ceremonies or gatherings.\n");
            break;
        case 4: // Exit Program
            printf("Exiting Program...\n");
            fclose(ifp);
            fclose(ofp);
            exit(0);
        default: // Loop if Not an Option
            printf("Not an option, Try Again\n");
            break;
        }
    }

    return 0;
}

Info Read_Entry(FILE *fileptr)
{
    Info entry;
    char coletter;
    // Read lot coordinates
    fscanf(fileptr, " %c-%d", &coletter, &entry.row);
    entry.column = toupper(coletter) - 'A';
    fscanf(fileptr, " %[^\n]", entry.fullname);
    fscanf(fileptr, "%d", &entry.BirthMonth);
    fscanf(fileptr, "%d", &entry.BirthDay);
    fscanf(fileptr, "%d", &entry.BirthYear);
    fscanf(fileptr, "%d", &entry.DeathMonth);
    fscanf(fileptr, "%d", &entry.DeathDay);
    fscanf(fileptr, "%d", &entry.DeathYear);
    fscanf(fileptr, " %[^\n]", entry.qoute);

    return entry;
}

Lot_Info Lot_details(FILE *lotptr)
{
    Lot_Info loentr;
    char lettercol;

    fscanf(lotptr, " %c-%d", &lettercol, &loentr.row);
    loentr.column = toupper(lettercol) - 'A';
    fscanf(lotptr, "%f", &loentr.size1);
    fscanf(lotptr, "%f", &loentr.size2);
    fscanf(lotptr, "%d", &loentr.Price);
    fscanf(lotptr, " %[^\n]", loentr.status);

    return loentr;
}

void MarkLot(char Lot[][Max_col], Info entry)
{
    Lot[entry.row - 1][entry.column] = 'X'; // Mark the lot based on 1-based indexing
}

void PLot(char Lot[][Max_col])
{
    printf("\t\t       A     B     C     D     E     F     G     H     I     J\n");
    printf("\t\t   --------------------------------------------------------------\n");
    for (int i = 0; i < Max_row; i++)
    {
        printf("\t\t%3d| ", i + 1);
        for (int j = 0; j < Max_col; j++)
        {
            printf("  %c  |", Lot[i][j]);
        }
        printf("\n\t\t   --------------------------------------------------------------\n");
    }
}

void cMonth(int num_month, char *monthptr)
{

    switch (num_month)
    {
    case 1:
        strcpy(monthptr, "January");
        break;
    case 2:
        strcpy(monthptr, "February");
        break;
    case 3:
        strcpy(monthptr, "March");
        break;
    case 4:
        strcpy(monthptr, "April");
        break;
    case 5:
        strcpy(monthptr, "May");
        break;
    case 6:
        strcpy(monthptr, "June");
        break;
    case 7:
        strcpy(monthptr, "July");
        break;
    case 8:
        strcpy(monthptr, "August");
        break;
    case 9:
        strcpy(monthptr, "September");
        break;
    case 10:
        strcpy(monthptr, "October");
        break;
    case 11:
        strcpy(monthptr, "November");
        break;
    case 12:
        strcpy(monthptr, "December");
        break;
    default:
        break;
    }
}

void Print_info(int col, int row, Info deads[MAX_ENT], Lot_Info lotdet[MAX_ENT])
{
    int index;
    char colleter;
    char reserve[MAX_STR] = "reserve";
    char bmonth[MAX_STR];
    char dmonth[MAX_STR];

    colleter = 'A' + col;

    index = ((row - 1) * Max_row) + col;
    printf("\n");

    cMonth(deads[index].BirthMonth, bmonth);
    cMonth(deads[index].DeathMonth, dmonth);

    if (strcmp(deads[index].fullname, "") != 0 && strcmp(lotdet[index].status, "taken") == 0)
    {
        printf("\n\nLot: %c-%d", colleter, deads[index].row);
        printf("\nName: %s", deads[index].fullname);
        printf("\nBirthday: %s/%d/%d", bmonth, deads[index].BirthDay, deads[index].BirthYear);
        printf("\nDate of Death: %s/%d/%d\n", dmonth, deads[index].DeathDay, deads[index].DeathYear);
        printf("Qoute: %s\n", deads[index].qoute);
    }
    else
    {
        printf("\n\nLot: %c-%d\n", colleter, lotdet[index].row);
        printf("\nSize: %.2fm x %.2fm\n", lotdet[index].size1, lotdet[index].size2);
        printf("Price: PHP%d\n", lotdet[index].Price);
        if (strcmp(lotdet[index].status, "reserve") == 0)
        {
            printf("Status: Reserve\n");
        }
        else if (strcmp(lotdet[index].status, "taken") == 0)
        {
            printf("Status: Taken\n");
        }
        else if (strcmp(lotdet[index].status, "free") == 0)
        {
            printf("Status: Free\n");
        }
    }
}
