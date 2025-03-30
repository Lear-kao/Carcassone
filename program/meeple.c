#include "../header/Carcassonne.h"

void nbMeepleVille( struct Grid *grille, int *nbmeeple, char coul_player)
{
    //pour l'instant la fonction ne fait que compter le nombre de meeple sans discinction
    if (grille->marquer == v_marquer) return;
    if(grille->tile->meeplePlace>0)*nbmeeple=*nbmeeple+1;
    grille->marquer = v_marquer;
    if( grille->tile->middle == VILLE )
    {
        if(grille->tile->top == VILLE)
        {
            nbMeepleVille(grille->top,&v_marquer,*nbmeeple);
        }
        if(grille->tile->right == VILLE)
        {
            nbMeepleVille(grille->right,&v_marquer,*nbmeeple);
            
        }
        if(grille->tile->left == VILLE)
        {
            nbMeepleVille(grille->left,&v_marquer,*nbmeeple);
        }
        
        if(grille->tile->bot == VILLE)
        {
            nbMeepleVille(grille->bot,&v_marquer,*nbmeeple);
        }
    }
}