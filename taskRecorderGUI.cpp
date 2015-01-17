/* 
 * Copyright (C) 2015 CODYCO Project
 * Author: Serena Ivaldi <serena.ivaldi@inria.fr>
 * website: www.codyco.eu
 *
 * Copyright (C) 2012 MACSi Project
 * Author: Serena Ivaldi
 * email:  serena.ivaldi@isir.upmc.fr
 * website: www.macsi.isir.upmc.fr
 *
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

/**
@ingroup macsi_guis

\defgroup record_grasps_gui recordGraspsGUI

A GUI for helping the user to record grasping data.

\section intro_sec Description

This GUI can be used to facilitate experiments of demonstrated grasps with iCub.
It opens a port and send recording commands to the \ref record_grasps module.
There is no feedback from the latter module in the current version.

\section lib_sec Libraries
- YARP libraries.
- GTK

\section tested_os_sec Tested OS
Linux

\author Serena Ivaldi
*/

#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/dev/all.h>
//
#include <gtk/gtk.h>
//
//#include <modHelp/modHelp.h>
//#include <macsi/objects/objects.h>
//
#include <string>
#include <iostream>

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
//using namespace modHelp;
//using namespace macsi::objects;
using namespace std;

#define displayValue(V) cout<<" "<< #V <<" : "<<V<<endl;
#define displayNameValue(S,V) cout<<" "<< S <<" : "<<V<<endl;
#define displayVector(V) cout<<" "<< #V <<" : "<<V.toString()<<endl;
#define displayNameVector(S,V) cout<<" "<< S <<" : "<<V.toString()<<endl;

 void readString(ResourceFinder &rf, string name, string &v, string vdefault)
{
if(rf.check(name.c_str()))
{
v = rf.find(name.c_str()).asString();
}
else
{
v = vdefault;
cout<<"Could not find value for "<<name<<". "
<<"Setting default "<<vdefault<<endl;
}
displayNameValue(name,v);
}


// the port used to send data
Port outPort;
std::string fileTrajectory = "";

// UTILS
//==================================================================
string i2s(int n)
{
    char buff [10];
    sprintf(buff,"%d",n);
    return string(buff);
}



// GTK
//==================================================================
// close window
void destroy (void)
{
    gtk_main_quit ();
}

//-----------------------------------------------------------
void button_start_click(GtkWidget *w)
{
    Bottle bot;
    gpointer file_entry = g_object_get_data (G_OBJECT(w), "file");
    const char* filename = gtk_entry_get_text(GTK_ENTRY(file_entry));
    bot.addString ("START_REC");
    bot.addString (filename); 
    cout << "Command sent :" << bot.toString () << endl;
    outPort.write (bot);

}

void button_stop_click( GtkWidget *widget)
{
    Bottle bot;
    bot.addString ("STOP_REC");
    cout << "Command sent :" << bot.toString () << endl;
    outPort.write (bot);
}




//=================================================================
//                     MAIN
//=================================================================

int main(int argc, char * argv[])
{
    //initialize yarp network
    Network yarp;

    // create device drivers and connect to icub
    if (!yarp.checkNetwork())
    {
        cout<<"YARP network not available. Aborting."<<endl;
        return -1;
    }
	
    ResourceFinder finder;
    //retrieve information for the list of parts
    finder.setVerbose(true);
    finder.setDefaultContext("taskRecorder");
    finder.setDefaultConfigFile("taskRecorder.ini");
    //finder.setDefaultContext("code_serena_icub");
    finder.configure(argc,argv);

    if (finder.check("help"))
    {
        cout << "Options:" << endl << endl;
        cout << "\t--from   fileName: input configuration file" << endl;
        cout << "\t--context dirName: resource finder context"  << endl;

        return 0;
    }

    Time::turboBoost();
    string moduleName;
    //string pathFiles= string (finder.getContextPath()+"/data");
    string pathFiles= string (finder.getHomeContextPath()+"/data");
    //pathFiles="/home/icub/Desktop/graspSaves"
    cout<<"Writing to path = "<<pathFiles<<endl;

    readString(finder,"name",moduleName,"taskRecorder");
    cout<<"TaskRecorderGUI: opening the port sending commands"<<endl;
    //opening port to send data
    outPort.open(string("/"+moduleName+"/gui:o").c_str());
    Network::connect(string("/"+moduleName+"/gui:o").c_str(),"/recordArms/rpc");

    //============ GTK ========================
    // creating the GTK window
    GtkWidget *window;
    GtkWidget *mainfixed;
    GtkWidget *mainTable;

    // This is called in all GTK applications. Arguments are parsed
    // from the command line and are returned to the application.
    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Arms Recording GUI");
    gtk_signal_connect (GTK_OBJECT (window), "destroy",GTK_SIGNAL_FUNC (destroy), NULL);
    gtk_container_border_width (GTK_CONTAINER (window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    mainfixed  = gtk_fixed_new ();
    gtk_widget_set_size_request (GTK_WIDGET(mainfixed), 300, 200);

    gtk_container_add (GTK_CONTAINER (window), mainfixed);

    mainTable = gtk_table_new (2, 2, true);

    gtk_fixed_put (GTK_FIXED(mainfixed), mainTable , 0, 0);

    // the entry for the speech text
    GtkWidget *file_edit = gtk_entry_new ();
    gtk_entry_set_editable(GTK_ENTRY(file_edit),true);
    gtk_entry_set_text(GTK_ENTRY(file_edit), "test.record");
    gtk_widget_set_size_request(file_edit, 150, 50);
    gtk_table_attach_defaults (GTK_TABLE (mainTable), file_edit, 1, 2, 0, 1);

    GtkWidget *file_label = gtk_label_new("Save to file:");
    gtk_widget_set_size_request(file_label, 150, 50);
    gtk_table_attach_defaults (GTK_TABLE (mainTable), file_label, 0, 1, 0, 1);

    GtkWidget *button_start = gtk_button_new_with_label("Start recording!");
    gtk_widget_set_size_request(button_start, 150, 50);
    g_signal_connect(G_OBJECT(button_start), "clicked", G_CALLBACK(button_start_click), NULL);
    gtk_table_attach_defaults (GTK_TABLE (mainTable), button_start, 0, 1, 1, 2);
    g_object_set_data (G_OBJECT(button_start), "file", (gpointer) file_edit);

    GtkWidget *button_stop = gtk_button_new_with_label("Stop recording");
    gtk_widget_set_size_request(button_stop, 150, 50);
    g_signal_connect(G_OBJECT(button_stop), "clicked", G_CALLBACK(button_stop_click), NULL);
    gtk_table_attach_defaults (GTK_TABLE (mainTable), button_stop, 1, 2, 1, 2);

    //-------------------------------------------------------------------------------------------
    // now we can plot in the window
    //-------------------------------------------------------------------------------------------

    gtk_widget_show_all (window);
    gtk_main ();

    //============ GTK ENDS ========================

    //-------------------------------------------------------------------------------------------
    // closing the GUI
    //-------------------------------------------------------------------------------------------

    //closing the thread to record..
    Bottle bot;
    bot.addString("quit");
    outPort.write(bot);
    cout<<"Sent message: "<<bot.toString()<<endl;

    cout<<"TaskRecorderGUI: closing the port sending commands"<<endl;
    //closing the port sending commands
    outPort.interrupt();
    outPort.close();

    cout<<"TaskRecorderGUI: finished"<<endl;

    return 1;
}


