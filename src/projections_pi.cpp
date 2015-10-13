/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Projections Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2015 by Sean D'Epagnier
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#include <wx/wx.h>
#include <wx/graphics.h>

#include "ProjectionsUI.h"
#include "PreferencesDialog.h"
#include "projections_pi.h"
#include "icons.h"

# if !defined(NAN)
# define NAN std::numeric_limits<double>::quiet_NaN ()
# endif

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new projections_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}


//-----------------------------------------------------------------------------
//
//    Projections PlugIn Implementation
//
//-----------------------------------------------------------------------------

projections_pi::projections_pi(void *ppimgr)
    :opencpn_plugin_111(ppimgr)
{
    // Create the PlugIn icons
    initialize_images();
}

int projections_pi::Init(void)
{
    m_PreferencesDialog = new PreferencesDialog(GetOCPNCanvasWindow());
    AddLocaleCatalog( _T("opencpn-projections_pi") );

    //    Get a pointer to the opencpn configuration object
    m_pConfig = GetOCPNConfigObject();

    LoadConfig();

    return (WANTS_CONFIG              |
            WANTS_PREFERENCES
            );
}

bool projections_pi::DeInit(void)
{
    SaveConfig();
    return true;
}

int projections_pi::GetAPIVersionMajor()
{
    return MY_API_VERSION_MAJOR;
}

int projections_pi::GetAPIVersionMinor()
{
    return MY_API_VERSION_MINOR;
}

int projections_pi::GetPlugInVersionMajor()
{
    return PLUGIN_VERSION_MAJOR;
}

int projections_pi::GetPlugInVersionMinor()
{
    return PLUGIN_VERSION_MINOR;
}

wxBitmap *projections_pi::GetPlugInBitmap()
{
    return _img_projections;
}

wxString projections_pi::GetCommonName()
{
    return _("Projections");
}

wxString projections_pi::GetShortDescription()
{
    return _("Projections plugin offers alternate projections");
}

wxString projections_pi::GetLongDescription()
{
    return _("Projections plugin offers alternative projections\n\
The projections can be configured by the user\n\
These projections may be useful for a more accurate interpretation of navigational data\n\
Mercator projections are especially skewed in high latitudes for examples\n\
\n\
  This plugin requires OpenGL enabled.\n");
}

void projections_pi::ShowPreferencesDialog( wxWindow* parent )
{
    m_PreferencesDialog->Show();
}


bool projections_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pConfig;

    if(!pConf)
        return false;

    pConf->SetPath( _T("/PlugIns/Projections") );

    return true;
}

bool projections_pi::SaveConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pConfig;

    if(!pConf)
        return false;

    pConf->SetPath( _T("/PlugIns/Projections") );
    
    return true;
}
