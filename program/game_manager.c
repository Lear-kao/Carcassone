#include "../header/Carcassonne.h"

int turnTraker = 0;
int finJeu = 0;
int nbPlayers = 0;
int nbBot = 0;
int v_marquer = 0;


int main()
{
    int gameRuns = 1;       // Vaut 1 le temps que le jeu doit continuer, 0 sinon.
    unsigned char nbHumanPlayers = 0; // Nombre de joueurs humain
    unsigned char nbBotPlayers = 0;   // Nombre de bots
    struct DLList *dllist = NULL; // set dans init_plateau
    struct list_player *p_list = NULL;
    struct Grid *leftTopGrid = NULL;
    struct Stack *stack = NULL;
    int hauteur = 1;
    int largeur = 1;

    printf("Texte de bienvenu");

    while (gameRuns)
    {
        // ----Phase 1 : Paramètrage de la partie----
        printf("\n1");
        stack = start_game(&p_list, &leftTopGrid, dllist, &hauteur, &largeur);

        // ---------------------------
        // ----Phase 2 : La partie----
        // ---------------------------
        printf("\n2");
        while (is_stack_not_empty(stack))
        {
            for (char i = 0; i < nbPlayers; i++) 
            {
                printf("\n%d",i+2);
                player_turn(i + 1, p_list,stack, &leftTopGrid, dllist, &hauteur, &largeur, p_list);
            }
        }
        // -------------------------------
        // ----Phase 3 : Fin de partie----
        // -------------------------------
        end_game_points_counter(*p_list);
        // free_Grid(&leftTopGrid);
        gameRuns = 0;
    }
}
