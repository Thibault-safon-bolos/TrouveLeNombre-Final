
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <gtk/gtk.h>

GtkStyleContext *context;
GtkCssProvider *provider;
GtkWidget *window;
GdkScreen *screen;
GtkWidget *grid;
GtkWidget *play;
GtkWidget *label;
GtkWidget *button;
GtkWidget *Champs;
int tentnb = 1;
int rnb = 10;
int nbr;
gboolean game_started = FALSE;



void CompaNb(int nbp, int nbb){
    char buf[100000];
    if (nbp > nbb)
    {
        gtk_entry_set_text(GTK_ENTRY(Champs), "");
        snprintf(buf, sizeof(buf), "Resultat: Nombre %d est Trop Grand.", nbp);
        gtk_label_set_text(GTK_LABEL(label), buf);
        tentnb++;
    }
    else if (nbp < nbb)
    {
        gtk_entry_set_text(GTK_ENTRY(Champs), "");
        snprintf(buf, sizeof(buf), "Resultat: Nombre %d est Trop Petit.", nbp);
        gtk_label_set_text(GTK_LABEL(label), buf);
        tentnb++;
    }
    else
    {
        snprintf(buf, sizeof(buf), "Resultat: Gagné le nombre était %d tu la trouver en %d tentative%s.\nEntrez une borne max, puis cliquez sur Replay.", nbb, tentnb, tentnb > 1 ? "s" : "");
        gtk_label_set_text(GTK_LABEL(label), buf);
        game_started = FALSE;
        gtk_button_set_label(GTK_BUTTON(play), "Replay");
        gtk_entry_set_text(GTK_ENTRY(Champs), "");
        gtk_widget_show(play);
        tentnb = 1;
    }
}



void ChoixBot(GtkWidget *widget, gpointer data){
    char buffer[50];
    sprintf(buffer, "Choisissez un nombre entre 0 et %d.", rnb);
    gtk_label_set_text(GTK_LABEL(label), buffer);
    game_started = TRUE;
    gtk_widget_hide(play);
    srand(time(NULL));    
    nbr = rand() % rnb;
}



void VerifNb(GtkWidget *widget, gpointer data){
    const gchar *texte_saisi = gtk_entry_get_text(GTK_ENTRY(Champs));
    char *endptr;
    errno = 0;
    long val = strtol(texte_saisi, &endptr, 10);
    if (errno == 0 && *endptr == '\0') {
        if (!game_started)
        {
            rnb = (int)val;
            char buffer[50];
            sprintf(buffer, "Cliquer sur Play pour lancer la partie.\nChoisissez un nombre entre 0 et %d.", rnb);
            gtk_label_set_text(GTK_LABEL(label), buffer);
            gtk_entry_set_text(GTK_ENTRY(Champs), "");

        }
        else
        {
            if (val>= 100000)
            {
                char buffer[50];sprintf(buffer, "Merci de rentrer un nombre plus petit.");
                gtk_label_set_text(GTK_LABEL(label), buffer);
            }
            else
            {
                int valeur = (int)val;
                CompaNb(valeur, nbr);
            }
        }

    } else {
        gtk_label_set_text(GTK_LABEL(label), "Resultat: erreur entrer un nombre valide.");   
        gtk_entry_set_text(GTK_ENTRY(Champs), "");
    }
}



int main(int argc, char *argv[]){
    //initialisation GTK
    gtk_init(&argc, &argv);

    //Creation d'une nouvelle fenetre
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //Set d'un ID pour CSS
    gtk_widget_set_name(window, "main_window");

    //Titre de la fenetre
    gtk_window_set_title(GTK_WINDOW(window), "Trouve le Nombre");

    //Set taille
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    gtk_window_set_icon_from_file(GTK_WINDOW(window), "logo.png", NULL);

    //Fermeture de la fentre propre
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //Création de l'application du css
    provider = gtk_css_provider_new();

    //Chemin du Css qui vient d'un fichier
    gtk_css_provider_load_from_path(provider,"style.css", NULL);

    //Set de l'écran
    screen = gdk_screen_get_default();

    //Application du CSS
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    //Creation d'une grille
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    label = gtk_label_new("Bienvenue ! Entrez une borne max, puis cliquez sur Play.");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1); // Col 0, Ligne 0, sur 2 hauteur et 1 largeurs

    Champs = gtk_entry_new();  // Création du champ de saisie

    gtk_grid_attach(GTK_GRID(grid), Champs, 0, 1, 1, 2);  // Ajout à la grille   
    
    button = gtk_button_new_with_label("Confirmation");
    gtk_grid_attach(GTK_GRID(grid), button, 2, 1, 2,2);   
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(VerifNb),NULL);

    play = gtk_button_new_with_label("Play");
    gtk_grid_attach(GTK_GRID(grid), play, 2, 4, 3, 1);    
    g_signal_connect(G_OBJECT(play), "clicked", G_CALLBACK(ChoixBot), NULL);

    //Montre les elements
    gtk_widget_show_all(window);
    //Maintien la fenetre ouverte
    gtk_main();    
    return 0;
}