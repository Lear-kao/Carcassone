#include "../header/Carcassonne.h"


char searchMeeple(struct Tile tile,int where)
{
    return tile.meeplePlace==where;
}

struct Grid* searchAbbaye(struct Grid* grille)
{
    if(grille->tile->middle == ABBAYES && !is_a_potential_tile(grille->tile)) return grille;
    if(grille->top->tile->middle == ABBAYES && !is_a_potential_tile(grille->top->tile)) return grille->top;
    if(grille->bot->tile->middle == ABBAYES && !is_a_potential_tile(grille->bot->tile)) return grille->bot;
    if(grille->left->tile->middle == ABBAYES && !is_a_potential_tile(grille->left->tile)) return grille->left;
    if(grille->right->tile->middle == ABBAYES && !is_a_potential_tile(grille->right->tile)) return grille->right;
    if(grille->top->left->tile->middle == ABBAYES && !is_a_potential_tile(grille->top->left->tile)) return grille->top->left;
    if(grille->top->right->tile->middle == ABBAYES && !is_a_potential_tile(grille->top->right->tile)) return grille->top->right;
    if(grille->bot->left->tile->middle == ABBAYES && !is_a_potential_tile(grille->bot->left->tile)) return grille->bot->left;
    if(grille->bot->right->tile->middle == ABBAYES && !is_a_potential_tile(grille->bot->right->tile)) return grille->bot->right;
    return NULL;
}

char where_is_meeple(  int type, struct Tile tile)
{
    switch (tile.meeplePlace)
    {
        case MP_RIGHT:
            if (type == ROUTE && (tile.right == VILLAGE || tile.right == ROUTE)) return 1;
            if (type == tile.right) return 1;
            if (type == BLASON && tile.right==VILLE) return 1;
            if (type == VILLE && tile.right==BLASON) return 1;
            break;
        case MP_TOP:
            if (type == ROUTE && (tile.top == VILLAGE || tile.top == ROUTE)) return 1;
            if (type == tile.top) return 1;
            if (type == BLASON && tile.top==VILLE) return 1;
            if (type == VILLE && tile.top==BLASON) return 1;            
            break;
        case MP_LEFT:
            if (type == ROUTE && (tile.left == VILLAGE || tile.left == ROUTE)) return 1;
            if (type == tile.left) return 1;
            if (type == BLASON && tile.left==VILLE) return 1;
            if (type == VILLE && tile.left==BLASON) return 1;            
            break;
        case MP_BOT:
            if (type == ROUTE && (tile.bot == VILLAGE || tile.bot == ROUTE)) return 1;
            if (type == tile.bot) return 1;
            if (type == BLASON && tile.bot==VILLE) return 1;
            if (type == VILLE && tile.bot==BLASON) return 1;
            break;
        case MP_MIDDLE:
            if (type == ROUTE && (tile.middle == VILLAGE || tile.middle == ROUTE)) return 1;
            if (type == tile.middle) return 1;
            if (type == BLASON && tile.middle==VILLE) return 1;
            if (type == VILLE && tile.middle==BLASON) return 1;
    }
    return 0;
}

char what_color_is_meeple(int color, struct Tile tile)
{
    if (tile.meeple == NULL) return 0;
    return tile.meeple->coulPlayer == color;
}

char nbMeepleVille(struct Grid *grille, int coul_player,enum places a)
{
    char res=0;
    grille->marquer = v_marquer;


    if(is_meeple_on_tile(grille->tile) && 
       grille->tile->meeple->coulPlayer == coul_player && 
       grille->tile->meeplePlace==a && 
       (where_is_meeple(VILLE,*(grille->tile)) || where_is_meeple(BLASON,*(grille->tile)))) res+=1;
    switch(a)
    {
        case RIGHT:
            grille->marquer=v_marquer;
            if(grille->right!=NULL && grille->tile->right==VILLE || grille->tile->right==BLASON)
            {
                res+=nbMeepleVilleEncap(grille->right,coul_player,MP_LEFT);
            }
            break;

        case TOP:
            grille->marquer=v_marquer;
            if(grille->top!=NULL && grille->tile->top==VILLE || grille->tile->top==BLASON)
            {
                res+=nbMeepleVilleEncap(grille->top,coul_player,MP_BOT);
            }
            break;

        case LEFT:
            grille->marquer=v_marquer;
            if(grille->left!=NULL && grille->tile->left==VILLE || grille->tile->left==BLASON)
            {
                res+=nbMeepleVilleEncap(grille->left,coul_player,MP_RIGHT);
            }
            break;
            
        case BOT:
            grille->marquer=v_marquer;
            if(grille->bot!=NULL && grille->tile->bot==VILLE || grille->tile->bot==BLASON)
            {
                res+=nbMeepleVilleEncap(grille->bot,coul_player,MP_TOP);
            }
            break;

        case MIDDLE:
            if(grille->tile->middle==VILLE || grille->tile->middle == BLASON)
            {
                res=nbMeepleVilleEncap(grille,coul_player,MP_MIDDLE);
            }
            break;
        

    }
    v_marquer++;
    printf("meeple=%d\n",res);
    return res;
}

char nbMeepleVilleEncap( struct Grid *grille, int coul_player, enum meeplePlace origin)
{
    char cmpt = 0;
    if (grille->marquer == v_marquer) return 0;
    grille->marquer = v_marquer;


    if(grille->tile->meeplePlace!=NO_MEEPLE)
    {
        if(grille->tile->middle == VILLE || grille->tile->middle==BLASON)
        {
            if (what_color_is_meeple(coul_player,*(grille->tile)) &&
                (where_is_meeple(VILLE,*(grille->tile)) || where_is_meeple(BLASON,*(grille->tile))))
                {
                    cmpt+= 1;
                }
        }

        else
        {
            if (what_color_is_meeple(coul_player,*(grille->tile)) &&
                (where_is_meeple(VILLE,*(grille->tile)) || where_is_meeple(BLASON,*(grille->tile))) &&
                searchMeeple(*(grille->tile),origin))
                {
                    cmpt+= 1;
                }
        }
    }    
    if( grille->tile->middle == VILLE || grille->tile->middle==BLASON)
    {
        if(grille->tile->top == VILLE || grille->tile->top==BLASON)
        {
            cmpt += nbMeepleVilleEncap(grille->top,coul_player,MP_BOT);
        }
        if(grille->tile->right == VILLE || grille->tile->right==BLASON)
        {
            cmpt += nbMeepleVilleEncap(grille->right,coul_player,MP_LEFT);
        }
        if(grille->tile->left == VILLE || grille->tile->left==BLASON)
        {
            cmpt += nbMeepleVilleEncap(grille->left,coul_player,MP_RIGHT);

        }
        if(grille->tile->bot == VILLE || grille->tile->bot==BLASON)
        {
            cmpt += nbMeepleVilleEncap(grille->bot,coul_player,MP_TOP);
        }
    }
    return cmpt;
}

char nbMeepleVille_nocolor(struct Grid *grille,enum places a)
{
    char res=0;
    grille->marquer = v_marquer;
    if(is_meeple_on_tile(grille->tile) &&
       grille->tile->meeplePlace==a &&
       (where_is_meeple(VILLE,*(grille->tile)) || where_is_meeple(BLASON,*(grille->tile)))) res+=1;

    switch(a)
    {
       case RIGHT:
            grille->marquer=v_marquer;
            if(grille->right!=NULL && grille->tile->right==VILLE || grille->tile->right==BLASON)
            {
               res+=nbMeepleVille_nocolorEncap(grille->right,MP_LEFT);
            }
            break;
   
        case TOP:
            grille->marquer=v_marquer;
            if(grille->top!=NULL && grille->tile->top==VILLE || grille->tile->top==BLASON)
            {
                res+=nbMeepleVille_nocolorEncap(grille->top,MP_BOT);
            }
            break;
   
        case LEFT:
            grille->marquer=v_marquer;
            if(grille->left!=NULL && grille->tile->left==VILLE || grille->tile->left==BLASON)
            {
               res+=nbMeepleVille_nocolorEncap(grille->left,MP_RIGHT);
            }
           break;
               
        case BOT:
            grille->marquer=v_marquer;
            if(grille->bot!=NULL && grille->tile->bot==VILLE || grille->tile->bot==BLASON)
            {
               res+=nbMeepleVille_nocolorEncap(grille->bot,MP_TOP);
            }
            break;
   
        case MIDDLE:
            if(grille->tile->middle==VILLE || grille->tile->middle == BLASON)
            {
               res=nbMeepleVille_nocolorEncap(grille,MP_MIDDLE);
            }
            break;
           
    }

    v_marquer++;
    printf("meeplenocolor=%d\n",res);
    return res;
}

char nbMeepleVille_nocolorEncap( struct Grid *grille,enum meeplePlace origin)
{
    char cmpt = 0;
    if (grille->marquer == v_marquer) return 0;
    grille->marquer = v_marquer;


    if(grille->tile->meeplePlace!=NO_MEEPLE) //temp devient grille
    {
        if(grille->tile->middle == VILLE || grille->tile->middle==BLASON)
        {
            if (where_is_meeple(VILLE,*(grille->tile)) || where_is_meeple(BLASON,*(grille->tile)))
                {
                    cmpt+= 1;
                }
        }

        else
        {
            if ((where_is_meeple(VILLE,*(grille->tile)) || where_is_meeple(BLASON,*(grille->tile))) &&
                searchMeeple(*(grille->tile),origin))
                {
                    cmpt+= 1;
                }
        }
    }   
    if( grille->tile->middle == VILLE || grille->tile->middle == BLASON)
    {
        if(grille->tile->top == VILLE || grille->tile->top==BLASON)
        {
            cmpt += nbMeepleVille_nocolorEncap(grille->top,MP_BOT);
        }
        if(grille->tile->right == VILLE || grille->tile->right == BLASON)
        {
            cmpt += nbMeepleVille_nocolorEncap(grille->right,MP_LEFT);
        }
        if(grille->tile->left == VILLE || grille->tile->left == BLASON)
        {
            cmpt += nbMeepleVille_nocolorEncap(grille->left,MP_RIGHT);

        }
        if(grille->tile->bot == VILLE || grille->tile->bot == BLASON)
        {
            cmpt += nbMeepleVille_nocolorEncap(grille->bot,MP_TOP);
        }
    }
    return cmpt;
}

char nbMeepleAbbaye( struct Grid *grille, int color)
/* 
Paramètre; la grille ou se situe l'abbaye, la couleur recherchée
Retour : la présence ou non d'un meeple dela couleur fournie
*/
{
    struct Grid *temp = searchAbbaye(grille);
    if(temp == NULL) return 0;
    if(is_meeple_on_tile(temp->tile))
    {
        if (what_color_is_meeple(color,*(temp->tile)) &&
            where_is_meeple(ABBAYES,*(temp->tile))) return 1;
    }
    return 0;
}

char nbMeepleAbbaye_nocolor(struct Grid *grille)
/* 
Paramètre; la grille ou se situe l'abbaye, la couleur recherchée
Retour : la présence ou non d'un meeple dela couleur fournie
*/
{
    struct Grid *temp = searchAbbaye(grille);
    if(temp == NULL) return 0;
    if(is_meeple_on_tile(temp->tile))
    {
        if (where_is_meeple(ABBAYES,*(temp->tile))) return 1;
    }
    return 0;
}

int max(char *list)
/* 
Prend en paramètre une liste d'entiers et renvoie le plus grand
*/
{
    int max = list[0];
    for( int i = 0; i<nbPlayers; i++)
    {
        if (max < list[i]) max = list[i];
    }
    return max;
}

char countMeepleRoad(struct Grid *grille, enum places start, int color)
{
    char  nbmeeple = 0;
    grille->marquer = v_marquer;
    if(is_meeple_on_tile(grille->tile))
    {
        if (what_color_is_meeple(color,*(grille->tile)) &&
            grille->tile->meeplePlace==start &&
            where_is_meeple(ROUTE,*(grille->tile))) nbmeeple += 1;
    }

    //grille->marquer = v_marquer;
    switch(start)
    {
        case RIGHT:
            grille->marquer=v_marquer;
            if(grille->right!=NULL && grille->tile->right==ROUTE)
            {
                nbmeeple+=meepleRoad(grille->right,color);
            }
            break;

        case TOP:
            grille->marquer=v_marquer;
            if(grille->top!=NULL && grille->tile->top==ROUTE)
            {
                nbmeeple+=meepleRoad(grille->top,color);
            }
            break;
        
        case LEFT:
            grille->marquer=v_marquer;
            if(grille->left!=NULL && grille->tile->left==ROUTE)
            {
                nbmeeple+=meepleRoad(grille->left,color);
            }
            break;

        case BOT:
            grille->marquer=v_marquer;
            if(grille->bot!=NULL && grille->tile->bot==ROUTE)
            {
                nbmeeple+=meepleRoad(grille->bot,color);
            }
            break;
        
        case MIDDLE:
            if(grille->tile->middle==ROUTE)
            {
                nbmeeple=meepleRoad(grille,color);
            }
    }

    printf("nbmeepleroute=%d\n",nbmeeple);

    v_marquer++;
    return nbmeeple;
}

char meepleRoad(struct Grid *grille, int color)
/* 
Compte le nombre de meeple sur une structure route en fonction de la couleur passé en paramètre.
*/
{
    char cmp=0;
    if( grille->marquer == v_marquer) return 0;
    grille->marquer = v_marquer;


    if(is_meeple_on_tile(grille->tile))
    {
        if(what_color_is_meeple(color,*(grille->tile)) &&
           where_is_meeple(ROUTE,*(grille->tile))){
            cmp+=1;
        }
    }

    if (grille->tile->middle == ROUTE)
    {
        if ( grille->tile->bot == ROUTE)
        {
            cmp += meepleRoad(grille->bot,color);
        }
        if ( grille->tile->left == ROUTE)
        {
            cmp += meepleRoad(grille->left,color);
        }
        if ( grille->tile->top == ROUTE)
        {
            cmp += meepleRoad(grille->top,color);
        }
        if ( grille->tile->right == ROUTE)
        {
            cmp += meepleRoad(grille->right,color);
        }
    }
    return cmp;
}

char countMeepleRoad_nocolor(struct Grid *grille, enum places start)
{
    char  nbmeeple = 0;
    grille->marquer = v_marquer;


    if(is_meeple_on_tile(grille->tile))
    {
        if (where_is_meeple(ROUTE,*(grille->tile)) &&
            grille->tile->meeplePlace == start ) nbmeeple += 1;
    }

    grille->marquer = v_marquer;
    switch(start)
    {
        case RIGHT:
            grille->marquer=v_marquer;
            if(grille->right!=NULL && grille->tile->right==ROUTE)
            {
                nbmeeple+=meepleRoad_nocolor(grille->right,MP_LEFT);
            }
            break;

        case TOP:
            grille->marquer=v_marquer;
            if(grille->top!=NULL && grille->tile->top==ROUTE)
            {
                nbmeeple+=meepleRoad_nocolor(grille->top,MP_BOT);
            }
            break;
        
        case LEFT:
            grille->marquer=v_marquer;
            if(grille->left!=NULL && grille->tile->left==ROUTE)
            {
                nbmeeple+=meepleRoad_nocolor(grille->left,MP_RIGHT);
            }
            break;

        case BOT:
            grille->marquer=v_marquer;
            if(grille->bot!=NULL && grille->tile->bot==ROUTE)
            {
                nbmeeple+=meepleRoad_nocolor(grille->bot,MP_TOP);
            }
            break;
        
        case MIDDLE:
            if(grille->tile->middle==ROUTE)
            {
                nbmeeple=meepleRoad_nocolor(grille,MP_MIDDLE);
            }
    }
    printf("nbmeepleroutenocolor=%d\n",nbmeeple);
    v_marquer++;
    return nbmeeple;
}

char meepleRoad_nocolor(struct Grid *grille,enum meeplePlace origin)
{
    char cmp=0;
    if( grille->marquer == v_marquer) return 0;
    grille->marquer = v_marquer;
    if(is_meeple_on_tile(grille->tile))
    {
        if(grille->tile->middle==ROUTE)
        {
            if(where_is_meeple(ROUTE,*(grille->tile)))
            {
                cmp+=1;
            }
        }

        else
        {
            if(where_is_meeple(ROUTE,*(grille->tile)) && searchMeeple(*(grille->tile),origin))
            {
                cmp+=1;
            }
        }
        
    }

    grille->marquer = v_marquer;

    if (grille->tile->middle == ROUTE)
    {
        if ( grille->tile->bot == ROUTE)
        {
            cmp += meepleRoad_nocolor(grille->bot,MP_TOP);
        }
        if ( grille->tile->left == ROUTE)
        {
            cmp += meepleRoad_nocolor(grille->left,MP_RIGHT);
        }
        if ( grille->tile->top == ROUTE)
        {
            cmp += meepleRoad_nocolor(grille->top,MP_BOT);
        }
        if ( grille->tile->right == ROUTE)
        {
            cmp += meepleRoad_nocolor(grille->right,MP_LEFT);
        }
    }

    return cmp;
}

int* where_i_can_put(struct Grid *grid)
{
    //int tab[5] = {RIEN,RIEN,RIEN,RIEN,RIEN};

    int *tab=(int*)malloc(5*sizeof(int));
    for( int i = 0; i < 5; i++) tab[i]=RIEN;
    printf("wtf");
    if((grid->tile->right == VILLE || grid->tile->right == BLASON) && !nbMeepleVille_nocolor(grid,RIGHT)) tab[0] = 1;
    else if(grid->tile->right == ROUTE && !countMeepleRoad_nocolor(grid,RIGHT)) tab[0] = 1;
    printf("right passé\n");
    if((grid->tile->top == VILLE || grid->tile->top == BLASON) && !nbMeepleVille_nocolor(grid,TOP)) tab[1] = 1;
    else if(grid->tile->top == ROUTE && !countMeepleRoad_nocolor(grid,TOP)) tab[1] = 1;
    printf("top passé\n");
    if((grid->tile->left == VILLE || grid->tile->left == BLASON) && !nbMeepleVille_nocolor(grid,LEFT)) tab[2] = 1;
    else if(grid->tile->left == ROUTE && !countMeepleRoad_nocolor(grid,LEFT)) tab[2] = 1;
    printf("left passé\n");
    if((grid->tile->bot == VILLE || grid->tile->bot == BLASON) && !nbMeepleVille_nocolor(grid,BOT)) tab[3] = 1;
    else if(grid->tile->bot == ROUTE && !countMeepleRoad_nocolor(grid,BOT)) tab[3] = 1;
    printf("bot passé\n");
    if((grid->tile->middle == VILLE || grid->tile->middle == BLASON) && !nbMeepleVille_nocolor(grid,MIDDLE)) tab[4] = 1;
    else if(grid->tile->middle == ROUTE && !countMeepleRoad_nocolor(grid,MIDDLE)) tab[4] = 1;
    else if(grid->tile->middle == ABBAYES) tab[4] = 1;
    printf("bot passé\n");

    if(grid->tile->middle==ROUTE && countMeepleRoad_nocolor(grid,MIDDLE))
    {
        if(grid->tile->right==ROUTE) tab[0]=RIEN;
        if(grid->tile->top==ROUTE) tab[1]=RIEN;
        if(grid->tile->left==ROUTE) tab[2]=RIEN;
        if(grid->tile->bot==ROUTE) tab[3]=RIEN;
    }

    if(grid->tile->middle==VILLE || grid->tile->middle==BLASON && nbMeepleVille_nocolor(grid,MIDDLE))
    {
        if(grid->tile->right==VILLE || grid->tile->right==BLASON) tab[0]=RIEN;
        if(grid->tile->top==VILLE || grid->tile->top==BLASON) tab[1]=RIEN;
        if(grid->tile->left==VILLE || grid->tile->left==BLASON) tab[2]=RIEN;
        if(grid->tile->bot==VILLE || grid->tile->bot==BLASON) tab[3]=RIEN;
    }
    return tab;
}

/* 
------------------------------------------------
Fonction pour enlever les meeples des structures
------------------------------------------------
*/
char removeMeepleVilleStart(struct Grid *grille,enum places a)
/* 
Argument:
    -Le bout  de la  grille ou commencer la recherche de meeple.
    -La direction de la structure à inspecter.
*/
{
    grille->marquer = v_marquer;
    if(is_meeple_on_tile(grille->tile) &&
       grille->tile->meeplePlace==a &&
       (where_is_meeple(VILLE,*(grille->tile)) || where_is_meeple(BLASON,*(grille->tile)))) remove_meeple(grille);

    switch(a)
    {
       case RIGHT:
            grille->marquer=v_marquer;
            if(grille->right!=NULL && grille->tile->right==VILLE || grille->tile->right==BLASON)
            {

               removeMeepleVille(grille->right,MP_LEFT);
            }
           break;
   
        case TOP:
           grille->marquer=v_marquer;
           if(grille->top!=NULL && grille->tile->top==VILLE || grille->tile->top==BLASON)
            {
                removeMeepleVille(grille->top,MP_BOT);
            }
           break;
   
        case LEFT:
            grille->marquer=v_marquer;
            if(grille->left!=NULL && grille->tile->left==VILLE || grille->tile->left==BLASON)
            {
                removeMeepleVille(grille->left,MP_RIGHT);
            }
           break;
               
        case BOT:
            grille->marquer=v_marquer;
            if(grille->bot!=NULL && grille->tile->bot==VILLE || grille->tile->bot==BLASON)
            {
                removeMeepleVille(grille->bot,MP_TOP);
            }
            break;
   
        case MIDDLE:
            if(grille->tile->middle==VILLE || grille->tile->middle == BLASON)
            {
                removeMeepleVille(grille,MP_MIDDLE);
            }
            break;
           
    }

    v_marquer++;
    return res;
}

void removeMeepleVille( struct Grid *grille, enum meeplePlace origin)
{
    char cmpt = 0;
    if (grille->marquer == v_marquer) return;
    grille->marquer = v_marquer;

    if(grille->tile->meeplePlace!=NO_MEEPLE) //temp devient grille
    {
        if(grille->tile->middle == VILLE || grille->tile->middle==BLASON)
        {
            if (where_is_meeple(VILLE,*(grille->tile)) || where_is_meeple(BLASON,*(grille->tile)))
                {
                    remove_meeple(grille);
                }
        }

        else
        {
            if ((where_is_meeple(VILLE,*(grille->tile)) || where_is_meeple(BLASON,*(grille->tile))) &&
                searchMeeple(*(grille->tile),origin))
                {
                    remove_meeple(grille);
                }
        }
    }   

    if( grille->tile->middle == VILLE || grille->tile->middle == BLASON)
    {
        if(grille->tile->top == VILLE || grille->tile->top==BLASON)
        {
            removeMeepleVille(grille->top,MP_BOT);
        }
        if(grille->tile->right == VILLE || grille->tile->right == BLASON)
        {
            removeMeepleVille(grille->right,MP_LEFT);
        }
        if(grille->tile->left == VILLE || grille->tile->left == BLASON)
        {
            removeMeepleVille(grille->left,MP_RIGHT);
        }
        if(grille->tile->bot == VILLE || grille->tile->bot == BLASON)
        {
            removeMeepleVille(grille->bot,MP_TOP);
        }
    }
    return void;
}

void removeMeepleRoadStart(struct Grid *grille)
{
    grille->marquer = v_marquer;

    if (is_meeple_on_tile(grille->tile)) {
        if (where_is_meeple(ROUTE, *(grille->tile))) {
            remove_meeple(grille);
        }
    }

    if (grille->tile->middle == ROUTE) {
        if (grille->bot && grille->tile->bot == ROUTE)
            removeMeepleRoad(grille->bot, MP_TOP);

        if (grille->left && grille->tile->left == ROUTE)
            removeMeepleRoad(grille->left, MP_RIGHT);

        if (grille->top && grille->tile->top == ROUTE)
            removeMeepleRoad(grille->top, MP_BOT);

        if (grille->right && grille->tile->right == ROUTE)
            removeMeepleRoad(grille->right, MP_LEFT);
    }
}


void removeMeepleRoad(struct Grid *grille, enum meeplePlace origin)
{
    if (grille->marquer == v_marquer) return;
    grille->marquer = v_marquer;

    if (is_meeple_on_tile(grille->tile))
    {
        if (grille->tile->middle == ROUTE)
        {
            if (where_is_meeple(ROUTE, *(grille->tile)))
            {
                remove_meeple(grille);
            }
        } 
        else 
        {
            if (where_is_meeple(ROUTE, *(grille->tile)) &&
                searchMeeple(*(grille->tile), origin))
            {
                remove_meeple(grille);
            }
        }
    }

    if (grille->tile->middle == ROUTE) {
        if (grille->bot && grille->tile->bot == ROUTE)
            removeMeepleRoad(grille->bot, MP_TOP);

        if (grille->left && grille->tile->left == ROUTE)
            removeMeepleRoad(grille->left, MP_RIGHT);

        if (grille->top && grille->tile->top == ROUTE)
            removeMeepleRoad(grille->top, MP_BOT);

        if (grille->right && grille->tile->right == ROUTE)
            removeMeepleRoad(grille->right, MP_LEFT);
    }
}


void removeMeepleAbbaye( struct Grid *grille)
/* 
Argument:
    -la grille ou se situe l'abbaye et  le meeple à retirer.
Retour :
    -Void
Description:
    Retire le  meeple de la geille si il est situé  sur une Abbaye.
*/
{
    struct Grid *temp = searchAbbaye(grille);
    if(temp == NULL) return 0;
    if(is_meeple_on_tile(temp->tile))
    {
        if (where_is_meeple(ABBAYES,*(temp->tile))) remove_meeple(grille);
    }
    return;
}

void remove_meeple(struct Grid *grille)
/* 
Argument:
    -Un pointeur sur la tuile  ou il faut retirer les  meeples.
Retour:
    -Void.
Description:
    La fonction remet le meeple dans  la réserve du  joueur puis le retire du la tuile.
*/
{
    grille->tile->meeple->nbMeeple += 1;
    grille->tile->meeple = NULL;
    grille->tile->meeplePlace = 0;
    return;
}