#include <gtk/gtk.h>

#define bufSize 1024

GtkBuilder *gtkBuilder;
GtkWidget *window;
GtkWidget *btn_cancelar;
GtkWidget *btn_procesar;
GtkWidget *rb_expropiativo;
GtkWidget *rb_noExpropiativo;
GtkWidget *fileChooser;
GtkWidget *btn_ChooseFile;
GtkWidget *btn_create;
GtkWidget ***threads;
GtkWidget *spn_numThreads;
GtkWidget *spn_quantum;
GtkWidget *SRWindowThreads;
GtkWidget **objetos;

char *filename;
int nObjetos;
// void executeFloyd(){
//     printf("FLoyd Func\n");
//     system("gcc -Wall -g -o testFloyd Floyd/FloydInterfaz.c Floyd/logicaFloyd.c `pkg-config --cflags --libs gtk+-3.0`-w -lm");
//     system("./testFloyd&");
   
// }//end execute floyd

void on_window_destroy (GtkWidget *object, gpointer user_data) {
    gtk_main_quit();
}


void loadFile(GtkWidget *widget, gpointer user_data){ 
    filename=gtk_file_chooser_get_filename (fileChooser);   
    printf("nombre archivo %s\n", gtk_file_chooser_get_filename (fileChooser));   
    FILE *fp;
    fp = fopen(filename, "r"); 
    char buf[bufSize];
   
    int lastLine = -1;
    int largo = 0;
    int lineFile = 0;

    while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
		if(lineFile == 0){
			if (strcmp(buf,"NoExp;") == 0){
		    	printf("No Exp\n");
		    	printf("aqui\n");
		    	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_noExpropiativo),TRUE);
		    }else if (strcmp(buf,"Exp;") == 0){
		    	printf("Exp\n");
		    	printf("aqui no\n");
		    	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_expropiativo),TRUE);
		    }
		}else if(lineFile == 1){
			int totalThreads = atoi(buf);	    
	        printf("after atoi: %d\n",totalThreads);	
	 	    gtk_spin_button_set_value(spn_numThreads,totalThreads);
	 	    lastLine = totalThreads + 2;

		}else if(lineFile == lastLine){
			printf("ultima linea\n");
			int quantum = atoi(buf);	    
	        printf("after atoi: %d\n",quantum);	
	 	    gtk_spin_button_set_value(spn_quantum,quantum);
		}
		printf("%s\n", buf);
		printf("LInea %d\n",lineFile );
		lineFile = lineFile + 1;
	}
	fclose(fp);
    gtk_widget_show_all (window); 

}//loadFIle

void processThreads(){

}

void generaThreadsInfo(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *table;

    printf(" Inside generaThreadsInfo\n");
    gint cantObjs = gtk_spin_button_get_value_as_int (spn_numThreads);
    nObjetos = cantObjs;
    printf(" SPinL %d\n",nObjetos );
    
    table = gtk_grid_new();
    threads= calloc(nObjetos+1,sizeof(GtkWidget));
    int i;
    for(i=0; i<nObjetos+1;i++)
        threads[i]=calloc(nObjetos,sizeof(GtkWidget));
    gtk_container_add (GTK_CONTAINER (SRWindowThreads), table);

    int x;
    int y;
    for(x=0;x<3;x++){ //columnas
       for(y=0; y<nObjetos+1;y++){ //filas
            GtkWidget *label;
            if(y==0){
            	if(x==0){
                    label=gtk_label_new("Thread");
                }else if(x==1){
                    label=gtk_label_new("Boletos");
                }else if(x==2){
                    label=gtk_label_new("Trabajo");
                }                
                gtk_grid_attach (GTK_GRID (table), label, x, y, 1, 1);
            }else{
          
                GtkWidget *entry;
                entry = gtk_entry_new ();
                gtk_grid_attach (GTK_GRID (table), entry, x, y, 1, 1); 
                printf("Almacenando en x= %d y y=%d\n",x,y-1);
                threads[y-1][x]= entry;      

                   
            }       
        }// end primer for
    }//end for  
    gtk_widget_show_all (window);  
}

int main(int argc, char *argv[])
{
   
    gtk_init(&argc, &argv);
    
    gtkBuilder = gtk_builder_new();
    gtk_builder_add_from_file(gtkBuilder, "LotterySchedulling2.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "LotterySchedulling"));

    btn_cancelar = GTK_WIDGET (gtk_builder_get_object (gtkBuilder, "btn_cancelar"));
    btn_procesar = GTK_WIDGET (gtk_builder_get_object (gtkBuilder, "btn_procesar"));
    fileChooser =  GTK_WIDGET (gtk_builder_get_object (gtkBuilder, "filechooser_scheduller"));
    btn_ChooseFile =  GTK_WIDGET (gtk_builder_get_object (gtkBuilder, "btn_ChooseFile"));
    btn_create =  GTK_WIDGET (gtk_builder_get_object (gtkBuilder, "btn_create"));
    spn_numThreads = GTK_WIDGET (gtk_builder_get_object (gtkBuilder, "spn_numThreads"));
    spn_quantum = GTK_WIDGET (gtk_builder_get_object (gtkBuilder, "spn_quantum"));
    
    SRWindowThreads = GTK_WIDGET (gtk_builder_get_object (gtkBuilder, "SRWindowThreads"));
    rb_expropiativo = GTK_WIDGET (gtk_builder_get_object (gtkBuilder, "rb_expropiativo"));
    rb_noExpropiativo = GTK_WIDGET (gtk_builder_get_object (gtkBuilder, "rb_noExpropiativo"));

    g_signal_connect (window, "destroy", G_CALLBACK (on_window_destroy), NULL);
    g_signal_connect (btn_cancelar, "clicked", G_CALLBACK (on_window_destroy), NULL);
    g_signal_connect (btn_ChooseFile, "clicked",G_CALLBACK (loadFile),NULL);
    g_signal_connect (btn_create, "clicked",G_CALLBACK (generaThreadsInfo),NULL);
    // g_signal_connect (knapSack, "clicked",G_CALLBACK (mochilaFunc),NULL);
    // g_signal_connect (seriesEquipos, "clicked",G_CALLBACK (seriesFunc),NULL);
    // g_signal_connect (rutasMasCortas, "clicked", G_CALLBACK (executeFloyd), NULL);    
    // g_signal_connect (reemplazo, "clicked",G_CALLBACK (reemplazoFunc),NULL);
    // g_signal_connect (arbolBin, "clicked", G_CALLBACK (executeArbolBin), NULL);

    gtk_widget_set_tooltip_text (btn_procesar, "Inicia la ejecución de los threads. ");  
    // gtk_widget_set_tooltip_text (knapSack, "Es un problema de optimización combinatoria, busca la mejor solución entre un conjunto de posibles soluciones a un problema."); 
    // gtk_widget_set_tooltip_text (reemplazo, "Es un algoritmo que busca minimizar el costo de reemplazos desde un instante especifico. "); 
    // //g_signal_connect_object (itemSalir, "activate",G_CALLBACK (on_window_destroy), (gpointer),NULL);
	//itemSalir
    g_object_unref(G_OBJECT(gtkBuilder));
    gtk_widget_show(window);
    gtk_main();
    
    return 0;
}