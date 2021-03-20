﻿using System;
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
using System.Data;

namespace mon_app_version_sqlite
{
    /// <summary>
    /// Logique d'interaction pour Pointage_par_employé.xaml
    /// </summary>
    public partial class Pointage_par_employé : Window
    {
        string dbConnectionString = "Data Source=C:\\Users\\DELL\\source\\repos\\mon_app_version_sqlite\\mon_app_database.db;Version=3;";
        public Pointage_par_employé()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e) //ok
        {
            SQLiteConnection sqlitecon = new SQLiteConnection(dbConnectionString); //création d'une connexion
            try
            {
                sqlitecon.Open(); //ouverture de la connexion créée
                string Querry = "select ID,Nom,Prénom,Date_et_heure from Pointage1 where ID='" + this.ID_a_afficher.Text + "' ";
                SQLiteCommand createCommand = new SQLiteCommand(Querry, sqlitecon);
                //toujours dans les arguments d'une commande SQLite: la requête sous forme de string, et la connexion déjà créée (de type SQLiteConnection)
                createCommand.ExecuteNonQuery();
                SQLiteDataAdapter dataAdp = new SQLiteDataAdapter(createCommand);
                DataTable dt = new DataTable("Pointage1");
                dataAdp.Fill(dt);
                dataGrid.ItemsSource = dt.DefaultView;
                dataAdp.Update(dt);
                sqlitecon.Close(); //fermeture de la connexion

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void DataGrid_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
           
        }
         
        private void Button_Click_1(object sender, RoutedEventArgs e) //retour au  menu
        {
            this.Hide();
            Menu menu = new Menu();
            menu.Show();
        }
    }
}
