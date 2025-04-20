#include "../header/Carcassonne.h"



int main()
{
    int gameRuns = 1;       // Vaut 1 le temps que le jeu doit continuer, 0 sinon.
    unsigned char nbHumanPlayers = 0; // Nombre de joueurs humain
    unsigned char nbBotPlayers = 0;   // Nombre de bots
    struct DLList *dllist = NULL;
    struct list_player *p_list = NULL;
    struct Grid *leftTopGrid = NULL;
    struct Stack *stack = NULL;
    int hauteur = 1;
    int largeur = 1;

    printf("Texte de bienvenu");

    while (gameRuns)
    {
        // ----Phase 1 : Paramètrage de la partie----

        stack = start_game(&p_list, &leftTopGrid);

        // ---------------------------
        // ----Phase 2 : La partie----
        // ---------------------------
        while (is_stack_not_empty(stack))
        {
            for (char i = 0; i < nbPlayers; i++) 
            {
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
