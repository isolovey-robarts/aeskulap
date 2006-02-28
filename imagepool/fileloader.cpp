/*
    Aeskulap ImagePool - DICOM abstraction library
    Copyright (C) 2005  Alexander Pipelka

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

    Alexander Pipelka
    pipelka@teleweb.at

    Last Update:      $Author: braindead $
    Update Date:      $Date: 2006/02/28 22:39:34 $
    Source File:      $Source: /cvsroot/aeskulap/aeskulap/imagepool/fileloader.cpp,v $
    CVS/RCS Revision: $Revision: 1.10.2.1 $
    Status:           $State: Exp $
*/

#include <gtkmm.h>

#include "dcfilefo.h"
#include "dcdeftag.h"

#include "imagepool.h"
#include "fileloader.h"
#include "poolstudy.h"

namespace ImagePool {

bool FileLoader::load(const std::list< Glib::ustring >& filelist) {
	if(busy() || filelist.size() == 0) {
		return false;
	}

	m_filelist = new std::list< Glib::ustring >(filelist);
	
	m_cache.clear();
	prescan_files(m_filelist);

	if(m_cache.size() == 0) {
		return false;
	}
	
	start();
	
	return true;
}

void FileLoader::prescan_files(std::list< Glib::ustring >* filelist) {
	std::string studyinstanceuid;
	std::list< Glib::ustring >::iterator i = filelist->begin();
	unsigned int curr = 0;
	unsigned int max = filelist->size();

	for(; i != filelist->end(); i++) {

		signal_prescan_progress((double)(++curr) / (double)max);

		DcmFileFormat dfile;
		OFCondition cond = dfile.loadFile(
							(*i).c_str(),
							EXS_Unknown,
							EGL_noChange,
							DCM_MaxReadLength,
							false);
	
		if(cond.good() && dfile.getDataset()->findAndGetOFString(DCM_StudyInstanceUID, studyinstanceuid).good()) {
			std::string seriesinstanceuid;
			dfile.getDataset()->findAndGetOFString(DCM_SeriesInstanceUID, seriesinstanceuid);

			m_cache[studyinstanceuid].m_instancecount++;
			m_cache[studyinstanceuid].m_seriesuid.insert(seriesinstanceuid);
			m_cache[studyinstanceuid].m_seriescount = m_cache[studyinstanceuid].m_seriesuid.size();
		}
	}
}

bool FileLoader::run() {
	std::list< Glib::ustring >* filelist = m_filelist;
	std::list< Glib::ustring >::iterator i = filelist->begin();
	std::string studyinstanceuid;

	for(; i != filelist->end(); i++) {
		DcmFileFormat dfile;
	
		OFCondition cond = dfile.loadFile(
							(*i).c_str(),
							EXS_Unknown,
							EGL_noChange,
							DCM_MaxReadLength,
							false);
		
		if(!cond.good()) {
			std::cout << "unable to open file !!!" << std::endl;
		}
		else {
			dfile.loadAllDataIntoMemory();
			std::cout << "opened file:" << (*i) << std::endl;
		
			DcmDataset* dset = dfile.getDataset();
			if(dset->findAndGetOFString(DCM_StudyInstanceUID, studyinstanceuid).good()) {
				add_image(dset);
			}
		}
	}
	
	delete filelist;
	m_filelist = NULL;
	
	return true;
}

} // namespace ImagePool
