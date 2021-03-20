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
using System.Windows.Shapes;
using System.Data.SQLite;

namespace mon_app_version_sqlite
{
    /// <summary>
    /// Logique d'interaction pour Modifier_un_employé.xaml
    /// </summary>
    public partial class Modifier_un_employé : Window
    {
        string dbConnectionString = "Data Source=C:\\Users\\DELL\\source\\repos\\mon_app_version_sqlite\\mon_app_database.db;Version=3;";

        public Modifier_un_employé()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e) //retour au menu
        {
            this.Hide();
            Menu menu = new Menu();
            menu.Show();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e) //boutton modifier
        {
            SQLiteConnection sqlitecon = new SQLiteConnection(dbConnectionString); //création d'une connexion
            try
            {
                sqlitecon.Open(); //ouverture de la connexion créée
                string Querry = "update Liste_des_employés set ID='"+this.ID.Text+"', Nom='"+this.Nom.Text+ "', Prénom='" + this.Prénom.Text + "', CIN='" + this.CIN.Text + "', Date_de_naissance='" + this.Date_de_naissance.Text + "', Téléphone='" + this.Téléphone.Text + "', Adresse='" + this.Adresse.Text + "' where ID='" + this.ID.Text + "'";
                //where ... càd modifier ces données où ...
                SQLiteCommand createCommand = new SQLiteCommand(Querry, sqlitecon);
                //toujours dans les arguments d'une commande SQLite: la requête sous forme de string, et la connexion déjà créée (de type SQLiteConnection)
                createCommand.ExecuteNonQuery();
                MessageBox.Show("Employé modifié avec suuccés");
                sqlitecon.Close(); //fermeture de la connexion

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }
    }
}
