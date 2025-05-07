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
/*
    Player : L'objet Player

    return :
    - 0 si le joueur n'a aucun Meeple
    - 1 si il reste au moins un Meeple au Joueur
*/
{
    printf("vraiment\n");
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

void put_meeple(struct Grid* grid, struct list_player *p_list, char pnumber)
{
    // problème aveclaliste  des  joueurs
    //printf("un crash est  généré par  la liste des  joueurs!!!\n");
    //printf("%d\n",p_list->player[pnumber]->nbMeeple);
    if (!is_meeple_on_player(p_list->player[pnumber]))
    {
        return;
    }
    printf("voulez vous poser un meeple ? (0 : Non, 1 : Oui)");
    int temp;
    scanf("%d",&temp);
    if(!temp) return;
    int meep_put = 0;
    int *tab_place = where_i_can_put(grid);
    while( !meep_put )
    {
        if(tab_place[0] != RIEN) printf("vous pouvez  placer à droite - 0\n");
        if(tab_place[1] != RIEN) printf("vous pouvez  placer en haut - 1\n");
        if(tab_place[2] != RIEN) printf("vous pouvez  placer à gauche - 2\n");
        if(tab_place[3] != RIEN) printf("vous pouvez  placer en bas - 3\n");
        if(tab_place[4] != RIEN) printf("vous pouvez  placer au millieu - 4\n");
        printf("sinon vous pouvez ne pas en placer - 5");
        scanf("%d",&temp);
        if(temp == 5) return;
        if(tab_place[temp] != RIEN)
        {
            grid->tile->meeplePlace = temp; 
            grid->tile->meeple = p_list->player[pnumber];
            meep_put = 1;
            p_list->player[pnumber]->nbMeeple-=1;
        }
        else printf("position invalide");

    }
    return;
}

void put_meeple_bot(struct Grid *grid,struct list_player *p_list, char pnumber)
{
    if(is_meeple_on_player(p_list->player[pnumber]))
    {
        int *tab_place=where_i_can_put(grid);

        int is_can_put=(tab_place[0]==1 || tab_place[1]==1 || tab_place[2]==1 || tab_place[3]==1 || tab_place[4]==1);


        if(is_can_put)
        {
            if(tab_place[4]==1 && grid->tile->middle==ABBAYES)
            {
                grid->tile->meeplePlace=MP_MIDDLE;
                grid->tile->meeple=p_list->player[pnumber];
                p_list->player[pnumber]->nbMeeple-=1;
            }//priorité abbaye

            else if(tab_place[4]==1 && (grid->tile->middle==VILLE || grid->tile->middle==BLASON))
            {
                grid->tile->meeplePlace=MP_MIDDLE;
                grid->tile->meeple=p_list->player[pnumber];
                p_list->player[pnumber]->nbMeeple-=1;
            }

            else{
                

                int where=-1;

                do{
                    where=rand()%5;
                }while(tab_place[where]==RIEN);

                grid->tile->meeple=p_list->player[pnumber];
                p_list->player[pnumber]->nbMeeple-=1;

                switch(where)
                {
                    case 0:
                        grid->tile->meeplePlace=MP_RIGHT;
                        break;
                    
                    case 1:
                        grid->tile->meeplePlace=MP_TOP;
                        break;
                    
                    case 2:
                        grid->tile->meeplePlace=MP_LEFT;
                        break;
                    
                    case 3:
                        grid->tile->meeplePlace=MP_BOT;
                        break;
                    
                    case 4:
                        grid->tile->meeplePlace=MP_MIDDLE;
                        break;

                }
            }
        }
        

        

    }
}




void remove_meeple(struct Grid *justPlaced, struct list_player *p_list)
{
    if(is_meeple_on_tile(justPlaced->tile))
    {
        p_list->player[justPlaced->tile->meeple->coulPlayer-1]->nbMeeple+=1;
        justPlaced->tile->meeple=NULL;
        justPlaced->tile->meeplePlace=NO_MEEPLE;
    }
}