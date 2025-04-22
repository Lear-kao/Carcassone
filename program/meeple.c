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
            break;
        case MP_TOP:
            if (type == ROUTE && (tile.top == VILLAGE || tile.top == ROUTE)) return 1;
            if (type == tile.top) return 1;            
            break;
        case MP_LEFT:
            if (type == ROUTE && (tile.left == VILLAGE || tile.left == ROUTE)) return 1;
            if (type == tile.left) return 1;            
            break;
        case MP_BOT:
            if (type == ROUTE && (tile.bot == VILLAGE || tile.bot == ROUTE)) return 1;
            if (type == tile.bot) return 1;
            break;
        case MP_MIDDLE:
            if (type == ROUTE && (tile.middle == VILLAGE || tile.middle == ROUTE)) return 1;
            if (type == tile.middle) return 1;
    }
    return 0;
}

char what_color_is_meeple(int color, struct Tile tile)
{
    return tile.meeple->coulPlayer == color;

}

char nbMeepleVille(struct Grid *grille, int coul_player){
    char res=nbMeepleVilleEncap(grille,coul_player);
    v_marquer++;
    return res;
}

char nbMeepleVilleEncap( struct Grid *grille, int coul_player)
{
    //pour l'instant la fonction ne fait que compter le nombre de meeple sans discinction
    char cmpt = 0;
    if (grille->marquer == v_marquer) return 0;
    if(is_meeple_on_tile(grille->tile)) //temp devient grille
    {
        if (what_color_is_meeple(coul_player,*(grille->tile)) &&
            where_is_meeple(VILLE,*(grille->tile))){
                cmpt+= 1;
            }
    }    
    grille->marquer = v_marquer;

    if( grille->tile->middle == VILLE )
    {
        if(grille->tile->top == VILLE)
        {
            cmpt += nbMeepleVilleEncap(grille->top,coul_player);
        }
        if(grille->tile->right == VILLE)
        {
            cmpt += nbMeepleVilleEncap(grille->right,coul_player);
        }
        if(grille->tile->left == VILLE)
        {
            cmpt += nbMeepleVilleEncap(grille->left,coul_player);

        }
        if(grille->tile->bot == VILLE)
        {
            cmpt += nbMeepleVilleEncap(grille->bot,coul_player);
        }
    }
    return cmpt;
}

char nbMeepleVille_nocolor(struct Grid *grille){
    char res=nbMeepleVille_nocolorEncap(grille,NO_MEEPLE);
    v_marquer++;
    return res;
}

char nbMeepleVille_nocolorEncap( struct Grid *grille,int where)
{
    //pour l'instant la fonction ne fait que compter le nombre de meeple sans discinction
    char cmpt = 0;
    if (grille->marquer == v_marquer) return 0;

    if(is_meeple_on_tile(grille->tile))
    {
        if(where_is_meeple(VILLE,*(grille->tile)) && 
          (searchMeeple(*(grille->tile),where) || searchMeeple(*(grille->tile),MP_MIDDLE))){
            cmpt+=1;
        }
    }

    grille->marquer = v_marquer;
    if( grille->tile->middle == VILLE )
    {
        if(grille->tile->top == VILLE)
        {
            cmpt += nbMeepleVille_nocolorEncap(grille->top,MP_BOT);
        }
        if(grille->tile->right == VILLE)
        {
            cmpt += nbMeepleVille_nocolorEncap(grille->right,MP_LEFT);
        }
        if(grille->tile->left == VILLE)
        {
            cmpt += nbMeepleVille_nocolorEncap(grille->left,MP_RIGHT);

        }
        if(grille->tile->bot == VILLE)
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
    
    if(is_meeple_on_tile(grille->tile))
    {
        if (what_color_is_meeple(color,*(grille->tile)) &&
            where_is_meeple(ROUTE,*(grille->tile))) nbmeeple += 1;
    }

    //grille->marquer = v_marquer;
    switch(start)
    {
        case RIGHT:
            nbmeeple += meepleRoad(grille->right,color);
            break;

        case TOP:
            nbmeeple += meepleRoad(grille->top,color);
            break;

        case LEFT:
            nbmeeple += meepleRoad(grille->left,color);
            break;

        case BOT:
            nbmeeple += meepleRoad(grille->bot,color);
            break;
    }

    v_marquer++;
    return nbmeeple;
}


char meepleRoad(struct Grid *grille, int color)// Commentaire à faire !!! unfinished initialisé à 1
{
    char cmp=0;
    if( grille->marquer == v_marquer) return 0;

    if(is_meeple_on_tile(grille->tile))
    {
        if(what_color_is_meeple(color,*(grille->tile)) &&
           where_is_meeple(ROUTE,*(grille->tile))){
            cmp+=1;
        }
    }

    grille->marquer = v_marquer;

    if (grille->tile->middle == ROUTE)
    {
        if ( grille->tile->bot == ROUTE && grille->bot !=NULL )
        {
            cmp += meepleRoad(grille->bot,color);
        }
        if ( grille->tile->left == ROUTE && grille->left !=NULL )
        {
            cmp += meepleRoad(grille->left,color);
        }
        if ( grille->tile->top == ROUTE && grille->top !=NULL )
        {
            cmp += meepleRoad(grille->top,color);
        }
        if ( grille->tile->right == ROUTE && grille->right !=NULL )
        {
            cmp += meepleRoad(grille->right,color);
        }
    }

    return cmp;
}




/* 
Fonction pour  savoir il y a  des meeples sur la route que fait les tuiles quel que soit la couleur
*/
char meepleRoad_nocolor(struct Grid *grille)// Commentaire à faire !!! unfinished initialisé à 1
{
    char cmp=0;
    if( grille->marquer == v_marquer) return 0;

    if(is_meeple_on_tile(grille->tile))
    {
        if(where_is_meeple(ROUTE,*(grille->tile))){
            cmp+=1;
        }
    }

    grille->marquer = v_marquer;

    if (grille->tile->middle == ROUTE)
    {
        if ( grille->tile->bot == ROUTE && grille->bot !=NULL )
        {
            cmp += meepleRoad_nocolor(grille->bot);
        }
        if ( grille->tile->left == ROUTE && grille->left !=NULL )
        {
            cmp += meepleRoad_nocolor(grille->left);
        }
        if ( grille->tile->top == ROUTE && grille->top !=NULL )
        {
            cmp += meepleRoad_nocolor(grille->top);
        }
        if ( grille->tile->right == ROUTE && grille->right !=NULL )
        {
            cmp += meepleRoad_nocolor(grille->right);
        }
    }

    return cmp;
}

char countMeepleRoad_nocolor(struct Grid *grille, enum places start)
{
    char  nbmeeple = 0;
    
    if(is_meeple_on_tile(grille->tile))
    {
        if (where_is_meeple(ROUTE,*(grille->tile))) nbmeeple += 1;
    }

    //grille->marquer = v_marquer;
    switch(start)
    {
        case RIGHT:
            nbmeeple += meepleRoad_nocolor(grille->right);
            break;

        case TOP:
            nbmeeple += meepleRoad_nocolor(grille->top);
            break;

        case LEFT:
            nbmeeple += meepleRoad_nocolor(grille->left);
            break;

        case BOT:
            nbmeeple += meepleRoad_nocolor(grille->bot);
            break;
    }

    v_marquer++;
    return nbmeeple;
}











int* where_i_can_put(struct Grid *grid)
{
    int tab[5] = {RIEN,RIEN,RIEN,RIEN,RIEN};
    if(grid->tile->right == VILLE && !nbMeepleVille_nocolor(grid)) tab[0] = 1;
    else if(grid->tile->right == ROUTE && !countMeepleRoad_nocolor(grid,RIGHT)) tab[0] = 1;
    if(grid->tile->top == VILLE && !nbMeepleVille_nocolor(grid)) tab[1] = 1;
    else if(grid->tile->top == ROUTE && !countMeepleRoad_nocolor(grid,TOP)) tab[1] = 1;
    if(grid->tile->left == VILLE && !nbMeepleVille_nocolor(grid)) tab[2] = 1;
    else if(grid->tile->left == ROUTE && !countMeepleRoad_nocolor(grid,LEFT)) tab[2] = 1;
    if(grid->tile->bot == VILLE && !nbMeepleVille_nocolor(grid)) tab[3] = 1;
    else if(grid->tile->bot == ROUTE && !countMeepleRoad_nocolor(grid,BOT)) tab[3] = 1;
    if(grid->tile->middle == VILLE && !nbMeepleVille_nocolor(grid)) tab[4] = 1;
    else if(grid->tile->middle == ROUTE && !countMeepleRoad_nocolor(grid,MIDDLE)) tab[4] = 1;
    else if(grid->tile->middle == ABBAYES) tab[4] = 1;
    return tab;
}