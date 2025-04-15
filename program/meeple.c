#include "../header/Carcassonne.h"


struct Grid* searchAbbaye(struct Grid* grille)
{
    if(grille->tile->middle == ABBAYES && !is_a_potential_tile(grille->top->tile)) return grille;
    if(grille->top->tile->middle == ABBAYES && !is_a_potential_tile(grille->top->tile)) return grille->top;
    if(grille->bot->tile->middle == ABBAYES && !is_a_potential_tile(grille->top->tile)) return grille->bot;
    if(grille->left->tile->middle == ABBAYES && !is_a_potential_tile(grille->top->tile)) return grille->left;
    if(grille->right->tile->middle == ABBAYES && !is_a_potential_tile(grille->top->tile)) return grille->right;
    if(grille->top->left->tile->middle == ABBAYES && !is_a_potential_tile(grille->top->tile)) return grille->top->left;
    if(grille->top->right->tile->middle == ABBAYES && !is_a_potential_tile(grille->top->tile)) return grille->top->right;
    if(grille->bot->left->tile->middle == ABBAYES && !is_a_potential_tile(grille->top->tile)) return grille->bot->left;
    if(grille->bot->left->tile->middle == ABBAYES && !is_a_potential_tile(grille->top->tile)) return grille->bot->right;
    return NULL;
}

char where_is_meeple(  int type, struct Tile tile)
{
    switch (tile.meeplePlace)
    {
        case RIGHT:
            if (type == ROUTE && (tile.right == VILLAGE || tile.right == ROUTE)) return 1;
            if (type == tile.right) return 1;
            break;
        case TOP:
            if (type == ROUTE && (tile.top == VILLAGE || tile.top == ROUTE)) return 1;
            if (type == tile.top) return 1;            
            break;
        case LEFT:
            if (type == ROUTE && (tile.left == VILLAGE || tile.left == ROUTE)) return 1;
            if (type == tile.left) return 1;            
            break;
        case BOT:
            if (type == ROUTE && (tile.bot == VILLAGE || tile.bot == ROUTE)) return 1;
            if (type == tile.bot) return 1;
            break;
        case MIDDLE:
            if (type == ROUTE && (tile.middle == VILLAGE || tile.middle == ROUTE)) return 1;
            if (type == tile.middle) return 1;
    }
    return 0;
}

char what_color_is_meeple(int color, struct Tile tile)
{
    return tile.meeple->coulPlayer == color;

}

char nbMeepleVille( struct Grid *grille, int coul_player)
{
    //pour l'instant la fonction ne fait que compter le nombre de meeple sans discinction
    char cmpt = 0;
    if (grille->marquer == v_marquer) return 0;
    if(is_meeple_on_tile(grille->tile)) //temp devient grille
    {
        if (what_color_is_meeple(coul_player,*(grille->tile)) &&
            where_is_meeple(VILLE,*(grille->tile))) cmpt+= 1;
    }    
    grille->marquer = v_marquer;

    if( grille->tile->middle == VILLE )
    {
        if(grille->tile->top == VILLE)
        {
            cmpt += nbMeepleVille(grille->top,coul_player);
        }
        if(grille->tile->right == VILLE)
        {
            cmpt += nbMeepleVille(grille->right,coul_player);
        }
        if(grille->tile->left == VILLE)
        {
            cmpt += nbMeepleVille(grille->left,coul_player);

        }
        if(grille->tile->bot == VILLE)
        {
            cmpt += nbMeepleVille(grille->bot,coul_player);
        }
    }
    return cmpt;
}

char nbMeepleVille_nocolor( struct Grid *grille)
{
    //pour l'instant la fonction ne fait que compter le nombre de meeple sans discinction
    char cmpt = 0;
    if (grille->marquer == v_marquer) return 0;
    grille->marquer = v_marquer;
    if( grille->tile->middle == VILLE )
    {
        if(grille->tile->top == VILLE)
        {
            cmpt += nbMeepleVille_nocolor(grille->top);
        }
        if(grille->tile->right == VILLE)
        {
            cmpt += nbMeepleVille_nocolor(grille->right);
        }
        if(grille->tile->left == VILLE)
        {
            cmpt += nbMeepleVille_nocolor(grille->left);

        }
        if(grille->tile->bot == VILLE)
        {
            cmpt += nbMeepleVille_nocolor(grille->bot);
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
        if (what_color_is_meeple(color,*(grille->tile)) &&
            where_is_meeple(ABBAYES,*(grille->tile))) return 1;
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
        if (where_is_meeple(ABBAYES,*(grille->tile))) return 1;
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
    switch(start)
    {
        case RIGHT:
            // if(grille->right == NULL  || is_a_potential_tile(grille->right->tile))
            // {
            //     point = 1;
            //     break;
            // }
            nbmeeple += meepleRoad(grille->right,color);
            break;
        case TOP:
            // if(grille->top == NULL  || is_a_potential_tile(grille->top->tile))
            // {
            //     point = 1;
            //     break;
            // }
            nbmeeple += meepleRoad(grille->top,color);
            break;
        case LEFT:
            // if(grille->left == NULL  || is_a_potential_tile(grille->left->tile))
            // {
            //     point = 1;
            //     break;
            // }
            nbmeeple += meepleRoad(grille->left,color);
            break;
        case BOT:
            // if(grille->bot == NULL  || is_a_potential_tile(grille->bot->tile))
            // {
            //     point = 1;
            //     break;
            // }
            nbmeeple += meepleRoad(grille->bot,color);
            break;
    }
    return nbmeeple;
}


char meepleRoad(struct Grid *grille, int color)// Commentaire à faire !!! unfinished initialisé à 1
{
    /*
    if( is_a_potential_tile(grille->tile) == 1)
    {
        return 0;
    }
    if( grille->marquer == v_marquer)
    {
        *unfinished = 0;
        return 0;
    }
    grille->marquer = v_marquer;
    char cmp = 1;
    if (grille->tile->meeple == ROUTE){
    if ( grille->tile->bot == ROUTE && grille->bot !=NULL )
    {
        cmp += isFinishedRoad(grille->bot,unfinished);
    }
    if ( grille->tile->left == ROUTE && grille->left !=NULL )
    {
        cmp += isFinishedRoad(grille->left,unfinished);
    }
    if ( grille->tile->top == ROUTE && grille->top !=NULL )
    {
        cmp += isFinishedRoad(grille->top,unfinished);
    }
    if ( grille->tile->right == ROUTE && grille->right !=NULL )
    {
        cmp += isFinishedRoad(grille->right,unfinished);
    }
    }
    if(grille->tile->middle == VILLAGE || grille->tile->middle == VILLE || grille->tile->middle == ABBAYES)
    {
        *unfinished = 0;
        return 1;
    }
    if (*unfinished == 1 && finJeu == 0)
    {
        return 0;
    }
    return cmp;
    */
}
