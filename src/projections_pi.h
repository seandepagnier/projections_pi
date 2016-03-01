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

#ifndef _PROJECTIONSPI_H_
#define _PROJECTIONSPI_H_

#include <wx/wx.h>

#include "version.h"

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    13

#define ABOUT_AUTHOR_URL "http://seandepagnier.users.sourceforge.net"

#include <wx/fileconf.h>
#include "ocpn_plugin.h"

#define PROJECTIONS_TOOL_POSITION -1          // Request default positioning of toolbar tool


//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

class PreferencesDialog;

enum Projection { MERCATOR, ORTHOGRAPHIC, POLAR, STEREOGRAPHIC, GNOMONIC, EQUIRECTANGULAR, NUM_PROJECTIONS };

class projections_pi : public wxEvtHandler, public opencpn_plugin_113
{
public:
    projections_pi( void *ppimgr );

//  The required PlugIn Methods
    int Init( void );
    bool DeInit( void );

    int GetAPIVersionMajor();
    int GetAPIVersionMinor();
    int GetPlugInVersionMajor();
    int GetPlugInVersionMinor();
    wxBitmap *GetPlugInBitmap();
    wxString GetCommonName();
    wxString GetShortDescription();
    wxString GetLongDescription();

    void SetCurrentViewPort(PlugIn_ViewPort &vp);
    void ShowPreferencesDialog( wxWindow* parent );
    void OnContextMenuItemCallback(int id);

private:

    bool LoadConfig( void );
    bool SaveConfig( void );

    wxFileConfig     *m_pConfig;

    wxMenuItem *m_projection_menuitem[NUM_PROJECTIONS];
    int m_projection_menu_id[NUM_PROJECTIONS];
};

#endif
