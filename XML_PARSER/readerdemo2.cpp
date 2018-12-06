

        // Load the xml file, I put your XML in a file named test.xml
        TiXmlDocument XMLdoc("test.xml");
        bool loadOkay = XMLdoc.LoadFile();
        if (loadOkay)
        {
            cout << "test.xml loaded" << endl;
            TiXmlElement *pRoot, *pParm, *pProcess, *pApp, *pLineFormat;
            pRoot = XMLdoc.FirstChildElement( "Configuration" );
            if ( pRoot )
            {
                // Parse parameters
                pParm = pRoot->FirstChildElement("Parameter");
                while ( pParm )
                {
                    cout << "Parameter: name= '" << pParm->Attribute("name") << "', value='" << pParm->Attribute("value") << "'" << endl;
                    pParm = pParm->NextSiblingElement( "Parameter" );
                }
     
                cout << endl;
     
                // Parse Process 
                pProcess = pRoot->FirstChildElement("Process" );
                while ( pProcess )
                {
                    cout <<"Process: " << pProcess->Attribute("Name" ) << endl;
                    pApp = pProcess->FirstChildElement("App" );
                    while ( pApp )
                    {
                        cout << "..App: ID=" << pApp->Attribute("ID") << ", name=" << pApp->Attribute("Name") << endl;
     
                        // ***************************************************
                        // * TODO: Put code in to handle the App details
                        // ***************************************************
     
                        pApp = pApp->NextSiblingElement("App");
                    }
     
                    pProcess = pProcess->NextSiblingElement("Process");
                }
            }
            else
            {
                cout << "Cannot find 'Configuration' node" << endl;
            }
     
     


