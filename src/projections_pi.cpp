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

static wxString projection_name[NUM_PROJECTIONS] = {
    _("Mercator"), _("Orthographic"), _("Polar"), _("Stereographic"), _("Gnomonic"), _("Equirectangular")};
static OcpnProjTypePI projection_map[NUM_PROJECTIONS] = {
      PI_PROJECTION_MERCATOR,
      PI_PROJECTION_ORTHOGRAPHIC,
      PI_PROJECTION_POLAR,
      PI_PROJECTION_STEREOGRAPHIC,
      PI_PROJECTION_GNOMONIC,
      PI_PROJECTION_EQUIRECTANGULAR
};

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
    :opencpn_plugin_113(ppimgr)
{
    // Create the PlugIn icons
    initialize_images();
}

int projections_pi::Init(void)
{
    AddLocaleCatalog( _T("opencpn-projections_pi") );

    //    Get a pointer to the opencpn configuration object
    m_pConfig = GetOCPNConfigObject();

    wxMenu *projection_menu = new wxMenu();
    for(int i=0; i<NUM_PROJECTIONS; i++) {
        m_projection_menuitem[i] = new wxMenuItem( projection_menu, -1, projection_name[i], wxEmptyString, wxITEM_CHECK );
        m_projection_menu_id[i] = AddCanvasContextMenuItem( m_projection_menuitem[i], this);
        SetCanvasContextMenuItemViz(m_projection_menu_id[i], false);
        projection_menu->Append(m_projection_menuitem[i]);
    }

    wxMenu dummy_menu;
    wxMenuItem *item = new wxMenuItem(&dummy_menu, -1, _("Projection"));
    item->SetSubMenu(projection_menu);
    AddCanvasContextMenuItem( item, this);
    
    LoadConfig();

    return (WANTS_CONFIG              |
            WANTS_ONPAINT_VIEWPORT    |
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
Mercator projections are especially skewed in high latitudes.\n\
\n\
  This plugin requires OpenGL enabled.\n");
}

void projections_pi::SetCurrentViewPort(PlugIn_ViewPort &vp)
{
    for(int i=0; i<NUM_PROJECTIONS; i++)
        m_projection_menuitem[i]->Check(projection_map[i] == vp.m_projection_type);
}

void projections_pi::ShowPreferencesDialog( wxWindow* parent )
{
    PreferencesDialog dlg(parent);
    dlg.ShowModal();
}

void projections_pi::OnContextMenuItemCallback(int id)
{
    for(int i=0; i<NUM_PROJECTIONS; i++)
        if(id == m_projection_menu_id[i])
            SetCanvasProjection(projection_map[i]);
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
