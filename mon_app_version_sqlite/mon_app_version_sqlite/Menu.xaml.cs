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
using System.Data;
using System.Data.SQLite;
using System.IO.Ports;
using System.Threading;
using System.Text.RegularExpressions;

namespace mon_app_version_sqlite
{
    /// <summary>
    /// Logique d'interaction pour Window1.xaml
    /// </summary>
    public partial class Menu : Window
    {
        SerialPort serialPort = new SerialPort("COM9",115200);
        string dbConnectionString = "Data Source=C:\\Users\\DELL\\source\\repos\\mon_app_version_sqlite\\mon_app_database.db;Version=3;";
        public Menu() //ici on va ouvrir le port série 
        {
            InitializeComponent();

            serialPort.Parity = Parity.None;
            serialPort.DataBits = 8;
            serialPort.StopBits = StopBits.One;
            serialPort.Handshake = Handshake.XOnXOff;
            serialPort.DtrEnable = true;
            serialPort.RtsEnable = true;
            serialPort.NewLine = Environment.NewLine;
            
        }

        private void Button_Click(object sender, RoutedEventArgs e) //Ajouter un emlpoyé
        {
            this.Hide(); //la fenêtre menu disparaît
            Ajouter_un_employé ajout = new Ajouter_un_employé();
            ajout.Show();

        }

        private void Button_Click_1(object sender, RoutedEventArgs e) //Modifier un emlpoyé
        {
           this.Hide(); //la fenêtre menu disparaît
           Modifier_un_employé mod = new Modifier_un_employé();
           mod.Show();
        }

        private void Button_Click_2(object sender, RoutedEventArgs e) //Supprimer un emlpoyé
        {
            this.Hide(); //la fenêtre menu disparaît
            Supprimer_un_employé supp = new Supprimer_un_employé();
            supp.Show();
        }

        private void Button_Click_3(object sender, RoutedEventArgs e) //liste des employés
        {
            this.Hide(); //la fenêtre menu disparaît
            Liste_des_employés liste = new Liste_des_employés();
            liste.Show();
        }

        private void Pointage_1_Click(object sender, RoutedEventArgs e) //Pointage1(Actualiser pointage)
        {
            //ici on va ouvrir le port série + Lecture
            try
            {
                serialPort.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Message", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            if (serialPort.IsOpen)
            {
                serialPort.Write("AT" + System.Environment.NewLine);
                Thread.Sleep(1000);
                serialPort.Write("AT+CMGF=1\r" + System.Environment.NewLine);
                Thread.Sleep(1000);
                serialPort.Write("AT+CPMS =\"SM\",\"SM\",\"SM\"\r" + System.Environment.NewLine);
                Thread.Sleep(2000);
                serialPort.Write("AT+CMGL=\"ALL\"\r" + System.Environment.NewLine); 
                Thread.Sleep(4000); //4000 c'est obligatoire !! 

                //MessageBox.Show(serialPort.ReadExisting());

                Regex r = new Regex(@"\+CMGL: (\d+),""(.+)"",""(.+)"",(.*),""(.+)""\r\n(.+)\r\n"); //expression régulière
                Match m = r.Match(serialPort.ReadExisting());
                while (m.Success)
                {
                    string a = m.Groups[1].Value; //indice du msg
                    string b = m.Groups[2].Value; //REC REAd ..
                    string c = m.Groups[3].Value; //numéro
                    string d = m.Groups[4].Value; //vide
                    string ee = m.Groups[5].Value; //date + heure
                    string f = m.Groups[6].Value; //contenu du msg


                    //vers BD table PortCOM
                    SQLiteConnection sqlitecon = new SQLiteConnection(dbConnectionString); //création d'une connexion
                    try
                    {
                        sqlitecon.Open(); //ouverture de la connexion créée
                        string Querry = "insert into PortCOM (indice,Statut,numéro,vide,Date_et_heure,Message)values('" + a + "','" + b + "','" + c + "','" + d + "','" + ee + "','" + f + "')";
                        SQLiteCommand createCommand = new SQLiteCommand(Querry, sqlitecon);
                        //toujours dans les arguments d'une commande SQLite: la requête sous forme de string, et la connexion déjà créée (de type SQLiteConnection)
                        createCommand.ExecuteNonQuery();
                        

                        if (c == "+21654650117") //vérifier le numéro du module GSM dans la pointeuse //A MODIFER !! 
                        {
                            string Querry2 = "select *from Liste_des_employés where ID='" + f + "'  "; //on cherche si Message=ID enregistré
                            SQLiteCommand createCommand2 = new SQLiteCommand(Querry2, sqlitecon);
                            createCommand2.ExecuteNonQuery();
                            SQLiteDataReader dr = createCommand2.ExecuteReader();

                            int count = 0;
                            while (dr.Read()) // Read() returns true if there is still a result line to read
                            {
                                count++;
                                //càd si on a trouvé ce qu'on a tapé dans la table, le compteur sera augmenté
                            }
                            if (count == 1)
                            //càd si i est différent de 0, cela veut dire qu'on a trouvé un ID enregistré 
                            {
                                //vers Table Pointage1
                                string Querry3 = "insert into Pointage1 (ID,Nom,Prénom,Date_et_heure) values ('" + f + "',(select Nom from Liste_des_employés where ID='" + f + "'), (select Prénom from Liste_des_employés where ID='" + f + "'), '" + ee + "') ";
                                SQLiteCommand createCommand3 = new SQLiteCommand(Querry3, sqlitecon);
                                createCommand3.ExecuteNonQuery();
                            }   
                        }

                        sqlitecon.Close(); //fermeture de la connexion

                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }

                    m = m.NextMatch();
                }

            }

            serialPort.Close(); //fermeture portCOM
            this.Hide(); //la fenêtre menu disparaît
            Pointage_1 pointage = new Pointage_1();
            pointage.Show();
        }

        private void Button_Click_4(object sender, RoutedEventArgs e) //Déconnexion
        {
            //Supprimer les messages dejà enregitrés dans la table Pointage1
            try
            {
                serialPort.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Message", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            if (serialPort.IsOpen)
            {
                serialPort.Write("AT" + System.Environment.NewLine);
                Thread.Sleep(1000);
                serialPort.Write("AT+CMGF=1\r" + System.Environment.NewLine);
                Thread.Sleep(1000);
                serialPort.Write("AT+CPMS=\"SM\",\"SM\",\"SM\"\r" + System.Environment.NewLine);
                Thread.Sleep(2000);
                serialPort.Write("AT+CMGD =1,4\r" + System.Environment.NewLine); //supprime tous les messages dans SM
                Thread.Sleep(4000);
                serialPort.Close(); //fermeture portCOM
            }

            //Vider la table PortCOM
            SQLiteConnection sqlitecon = new SQLiteConnection(dbConnectionString); //création d'une connexion
            try
            {
                sqlitecon.Open(); //ouverture de la connexion créée
                string Querry = "delete from PortCOM";
                SQLiteCommand createCommand = new SQLiteCommand(Querry, sqlitecon);
                createCommand.ExecuteNonQuery();
                sqlitecon.Close(); //fermeture de la connexion

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

            //fermeture et retour au login
            this.Close(); 
            MainWindow main = new MainWindow();
            main.Show();
        }

        private void Button_Click_5(object sender, RoutedEventArgs e) // Pointage par employé
        {
            this.Hide(); //la fenêtre menu disparaît
            Pointage_par_employé pointage_jour = new Pointage_par_employé();
            pointage_jour.Show();
        }
    }
}
