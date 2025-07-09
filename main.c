
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <gtk/gtk.h>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")  // Nécessaire pour linker winmm.lib

GtkStyleContext *context;
GtkCssProvider *provider;
GtkWidget *window;
GdkScreen *screen;
GtkWidget *overlay;
GtkWidget *fixed;
GtkWidget *play;
GtkWidget *label;
GtkWidget *button;
GtkWidget *Champs;
GtkWidget *buttonmore1;
GtkWidget *buttonmore5;
GtkWidget *buttonless1;
GtkWidget *buttonless5;
GtkStyleContext *context;

int tentnb = 1;
int rnb = 10;
int nbr;
gboolean game_started = FALSE;

void jouer_son_depuis_exe(const char *fichier) {
    char chemin_exe[MAX_PATH];
    GetModuleFileNameA(NULL, chemin_exe, MAX_PATH);

    // Trouver le dossier
    char *dernier_slash = strrchr(chemin_exe, '\\');
    if (dernier_slash) *(dernier_slash + 1) = '\0'; // garde le chemin du dossier

    // Concatène le chemin complet
    strcat(chemin_exe, fichier);

    PlaySound(chemin_exe, NULL, SND_FILENAME | SND_ASYNC);
}

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
        jouer_son_depuis_exe("sons/victoire.wav");
        game_started = FALSE;
        gtk_button_set_label(GTK_BUTTON(play), "Replay");
        gtk_entry_set_text(GTK_ENTRY(Champs), "");
        gtk_widget_show(play);
        tentnb = 1;
    }
}

void buttonaddorless(GtkWidget *widget, gpointer data){
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(Champs));
    int nombre = atoi(text); 
    char nouveau_texte[16];
    if (widget == buttonmore1)
        nombre += 1;
    else if (widget == buttonmore5)
        nombre += 5;
    else if (widget == buttonless1)
        nombre -= 1;
    else if (widget == buttonless5)
        nombre -= 5;
    snprintf(nouveau_texte, sizeof(nouveau_texte), "%d", nombre);
    gtk_entry_set_text(GTK_ENTRY(Champs), nouveau_texte);
}


void ChoixBot(GtkWidget *widget, gpointer data){
    jouer_son_depuis_exe("sons/let's_go.wav");
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

    overlay = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(window), overlay);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(overlay), fixed);
    
    label = gtk_label_new("Bienvenue ! Entrez une borne max, puis cliquez sur Play.");
    gtk_fixed_put(GTK_FIXED(fixed), label, 20, 20);  // X, Y à ajuster

    Champs = gtk_entry_new();  // Création du champ de saisie
    gtk_entry_set_placeholder_text(GTK_ENTRY(Champs), "Entrez une borne max, puis cliquez sur Play.");
    gtk_widget_set_size_request(Champs, 350, -1);
    gtk_fixed_put(GTK_FIXED(fixed), Champs, 20, 60);  // Position X, Y à ajuster

    buttonless5 = gtk_button_new_with_label("-5");
    gtk_fixed_put(GTK_FIXED(fixed), buttonless5, 20, 100);

    buttonless1 = gtk_button_new_with_label("-1");
    gtk_fixed_put(GTK_FIXED(fixed), buttonless1, 110, 100);

    buttonmore1 = gtk_button_new_with_label("+1");
    gtk_fixed_put(GTK_FIXED(fixed), buttonmore1, 200, 100);

    buttonmore5 = gtk_button_new_with_label("+5");
    gtk_fixed_put(GTK_FIXED(fixed), buttonmore5, 290, 100);

    g_signal_connect(buttonmore1, "clicked", G_CALLBACK(buttonaddorless), NULL);
    g_signal_connect(buttonmore5, "clicked", G_CALLBACK(buttonaddorless), NULL);
    g_signal_connect(buttonless1, "clicked", G_CALLBACK(buttonaddorless), NULL);
    g_signal_connect(buttonless5, "clicked", G_CALLBACK(buttonaddorless), NULL);



    context = gtk_widget_get_style_context(buttonless5);
    gtk_style_context_add_class(context, "small-button");

    context = gtk_widget_get_style_context(buttonless1);
    gtk_style_context_add_class(context, "small-button");

    context = gtk_widget_get_style_context(buttonmore1);
    gtk_style_context_add_class(context, "small-button");

    context = gtk_widget_get_style_context(buttonmore5);
    gtk_style_context_add_class(context, "small-button");

    
    button = gtk_button_new_with_label("Confirmation");
    gtk_fixed_put(GTK_FIXED(fixed), button, 400, 60);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(VerifNb),NULL);

    play = gtk_button_new_with_label("Play");
    gtk_fixed_put(GTK_FIXED(fixed), play, 400, 100);
    g_signal_connect(G_OBJECT(play), "clicked", G_CALLBACK(ChoixBot), NULL);

    //Montre les elements
    gtk_widget_show_all(window);
    //Maintien la fenetre ouverte
    gtk_main();    
    return 0;
}