#include "../header/Carcassonne.h"



void main()
{
    char gameRuns = 1;       // Vaut 1 le temps que le jeu doit continuer, 0 sinon.
    char nbHumanPlayers = 0; // Nombre de joueurs humain
    char nbBotPlayers = 0;   // Nombre de bots
    printf("Texte de bienvenu");

    while (gameRuns)
    {
        // ----Phase 1 : Paramètrage de la partie----
        char phase1 = 1;
        while (phase1)
        {
            printf("Entrez le nombre de joueurs humains : ");
            scanf("%u", &nbHumanPlayers);
            printf("Entrez le nombre de robots participant à la partie : ");
            scanf("%u", &nbBotPlayers);
            nbPlayers = nbHumanPlayers + nbBotPlayers;
            if ((nbPlayers) <= 6) // Phase 1 terminé
            {
                phase1 = 0;
            }
            else // boucle sur Phase 1
            {
                printf("Le nombre de joueurs et de bot doit être compris entre 0 et 6\n");
            }
        }

        // ----------------------------------------
        // ----Phase 2 : Lancement de la partie----
        // ----------------------------------------
        
        // Dans start game
        //struct **create_players_array(char nbPlayers);
        //struct Tile **create_tile_array(FILE * stream);
        //void shuffle(struct Tile * *tileArray, char size);
        //void array_to_stack(struct Tile * *tileArray, struct Stack * stack);
        // Fin dans start game
        struct player_list *p_list;
        struct Grid *grid;
        start_game(p_list, grid);        
        // ---------------------------
        // ----Phase 3 : La partie----
        // ---------------------------
        show_grid();
        while (is_stack_not_empty(stack))
        {
            for (char i = 0; i < nbPlayers; i++) 
            {
                player_turn(i);
                show_grid(Grid);
            }
        }

        // -------------------------------
        // ----Phase 4 : Fin de partie----
        // -------------------------------
        char *end_game_points_counter(struct Grid *grid, struct Player nbPlayers);
        printf("voulez-vous continuer?(0:non/1:oui)\n");
        free_Grid(grid);
        scanf("%d",gameRuns);
    }
}