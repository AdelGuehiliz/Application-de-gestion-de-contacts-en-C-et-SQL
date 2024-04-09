using System;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Windows.Forms;

namespace GestionContacts
{
    public partial class MainForm : Form
    {
        private SQLiteConnection connection;

        public MainForm()
        {
            InitializeComponent();
            connection = new SQLiteConnection("Data Source=contacts.db");
            connection.Open();
            InitializeDatabase();
            LoadContacts();
        }

        private void InitializeDatabase()
        {
            string createTableQuery = "CREATE TABLE IF NOT EXISTS Contacts (Id INTEGER PRIMARY KEY, Name TEXT, PhoneNumber TEXT, Email TEXT)";
            SQLiteCommand command = new SQLiteCommand(createTableQuery, connection);
            command.ExecuteNonQuery();
        }

        private void LoadContacts()
        {
            contactsListBox.Items.Clear();
            string query = "SELECT * FROM Contacts";
            SQLiteCommand command = new SQLiteCommand(query, connection);
            SQLiteDataReader reader = command.ExecuteReader();
            while (reader.Read())
            {
                string name = reader["Name"].ToString();
                string phoneNumber = reader["PhoneNumber"].ToString();
                string email = reader["Email"].ToString();
                Contact contact = new Contact(name, phoneNumber, email);
                contactsListBox.Items.Add(contact);
            }
            reader.Close();
        }

        private void addContactButton_Click(object sender, EventArgs e)
        {
            string name = nameTextBox.Text;
            string phoneNumber = phoneNumberTextBox.Text;
            string email = emailTextBox.Text;
            if (!string.IsNullOrEmpty(name) && !string.IsNullOrEmpty(phoneNumber))
            {
                string insertQuery = "INSERT INTO Contacts (Name, PhoneNumber, Email) VALUES (@Name, @PhoneNumber, @Email)";
                SQLiteCommand command = new SQLiteCommand(insertQuery, connection);
                command.Parameters.AddWithValue("@Name", name);
                command.Parameters.AddWithValue("@PhoneNumber", phoneNumber);
                command.Parameters.AddWithValue("@Email", email);
                command.ExecuteNonQuery();
                LoadContacts();
                ClearTextBoxes();
            }
            else
            {
                MessageBox.Show("Name and phone number are required fields.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void ClearTextBoxes()
        {
            nameTextBox.Text = "";
            phoneNumberTextBox.Text = "";
            emailTextBox.Text = "";
        }
    }

    public class Contact
    {
        public string Name { get; set; }
        public string PhoneNumber { get; set; }
        public string Email { get; set; }

        public Contact(string name, string phoneNumber, string email)
        {
            Name = name;
            PhoneNumber = phoneNumber;
            Email = email;
        }

        public override string ToString()
        {
            return $"{Name} - {PhoneNumber} - {Email}";
        }
    }
}
