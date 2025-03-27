#include "../header/Carcassonne.h"

char nbMeepleVille( struct Grid *grille, int coul_player)
{
    //pour l'instant la fonction ne fait que compter le nombre de meeple sans discinction
    cmpt = 0;
    if (grille->marquer == v_marquer) return;
    if(grille->tile->meeplePlace>0 ) cmpt = 1;
    grille->marquer = v_marquer;
    if( grille->tile->middle == VILLE )
    {
        if(grille->tile->top == VILLE)
        {
            cmpt += nbMeepleVille(grille->top,nbmeeple);
        }
        if(grille->tile->right == VILLE)
        {
            cmpt += nbMeepleVille(grille->right,nbmeeple);
            
        }
        if(grille->tile->left == VILLE)
        {
            cmpt += nbMeepleVille(grille->left,nbmeeple);
        }
        
        if(grille->tile->bot == VILLE)
        {
            cmpt += nbMeepleVille(grille->bot,nbmeeple);
        }
    }
    return cmpt;
}


/* char *list = malloc(sizeof(char) * nbJoueur)
for( int i = 0; i < nbjoueur ; i++)
{
    list[i] = nbMeepleVille(grille, i);
} */