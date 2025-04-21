#include "../header/Carcassonne.h"

// ---------------
// ----Player-----
// ---------------

struct Player *init_player(int couleur,char bot)
{
    /*
        Innitialise l'objet player
    */
    struct Player *player=malloc(sizeof(struct Player));

    player->bot=bot;
    player->coulPlayer = couleur;  
    player->nbMeeple = NBMEEPLE_DEFAULT;
    player->points = 0;
    return player;
}

char is_meeple_on_player(struct Player *player)
{/*
    Player : L'objet Player

    return :
    - 0 si le joueur n'a aucun Meeple
    - 1 si il reste au moins un Meeple au Joueur
*/
    return player->nbMeeple;
}

struct list_player *init_player_list(char nbbot) // Axel
/*
    Crée une liste de pointeurs qui pointe sur un Player,
    un pointer sur NULL est ajouté à la fin pour faciliter
    les iterations sur la liste (condition d'arrêt)
    Nbplayer est une variable globale 
*/
{
    struct list_player *list_players = (struct list_player*)malloc(sizeof(struct list_player));
    list_players->player = (struct Player**)malloc((nbPlayers + 1)*sizeof(struct Player*));
    for( int i = 0; i < nbPlayers; i++ )
    {
        if(i>nbPlayers-nbbot-1){
            list_players->player[i]=init_player(i+1,1);
        }
        else{
            list_players->player[i]=init_player(i+1,0);
        }
    }
    list_players->player[nbPlayers] = NULL;
    return list_players;
}


void reset_points(struct Player *player) // Fait
/*
    Réinitialise les points du joueur
*/
{
    player->points = 0;
}

void reset_meeples(struct Player *player) // Fait
/*
    Réinitialise le compteur de Meeple du joueur
*/
{
    player->nbMeeple = NBMEEPLE_DEFAULT;
}


//fonction pas finie je dois faire bcp de fct  à côté

/* void put_meeple(struct Grid* grid, struct list_player *p_list, char pnumber)
{
    //if (!can_place_meeple()){ printf("vous ne pouvez pas poser de meeple\n");return;}
    if (!is_meeple_on_player(p_list->player[pnumber]))
    {
        return;
    }
    printf("voulez vous poser un meeple ? (0 : Non, 1 : Oui)");
    int temp;
    scanf("%d",temp);
    if(temp) return;
    printf("ou voulez vous poser votre meeple (0 = Droite, 1 = Haut, 2 = Gauche, 3 = Bas)");
    scanf("%c",grid->tile->meeplePlace);
    grid->tile->meeple = p_list->player[pnumber];
    return;
} */