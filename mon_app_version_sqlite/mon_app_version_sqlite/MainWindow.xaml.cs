using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Data.SQLite;

namespace mon_app_version_sqlite
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        string dbConnectionString = "Data Source=C:\\Users\\DELL\\source\\repos\\mon_app_version_sqlite\\mon_app_database.db;Version=3;"; 
        //si la db est dans debud de la solution, on peut ne pas mettre le chemin complet
        public MainWindow()
        {
            InitializeComponent(); //par défaut génère les codes des éléments ajoutés au fichier.xaml
        }

        private void Button_Click(object sender, RoutedEventArgs e) //Bouton Exit
        {
            this.Close(); //à vérifier si on veut ce bouton ou non ! 
        }

        private void Button_Click_1(object sender, RoutedEventArgs e) //Bouton Connexion
        {
            SQLiteConnection sqlitecon = new SQLiteConnection(dbConnectionString); //création d'une connexion
            try
            {
                sqlitecon.Open(); //ouverture de la connexion créée
                string Querry = "select * from LOGIN where Nom_d_admin='" + TextBox1.Text + "' and Mot_de_passe='" + PasswordBox1.Password + "' ";
                SQLiteCommand createCommand = new SQLiteCommand(Querry, sqlitecon);
                //toujours dans les arguments d'une commande SQLite: la requête sous forme de string, et la connexion déjà créée (de type SQLiteConnection)
                createCommand.ExecuteNonQuery();
                SQLiteDataReader dr = createCommand.ExecuteReader();

                int count = 0;
                while (dr.Read()) // Read() returns true if there is still a result line to read
                {
                    count++; 
                    //càd si on a trouvé ce qu'on a tapé dans la table, le compteur sera augmenté
                }
                if (count==1) 
                    //càd si i est différent de 0, cela veut dire qu'on a trouvé les cordonnées tapés, donc le login se fait 
                {
                    this.Hide();
                    sqlitecon.Close(); //fermeture de la connexion
                    Menu menu = new Menu();
                    menu.Show();
                    sqlitecon.Close(); //fermeture de la connexion
                }
                else
                {
                    MessageBox.Show("Erreur ! Veuillez vérifier votre nom et votre mot de passe ! ");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
           
       
        }
    }
}
