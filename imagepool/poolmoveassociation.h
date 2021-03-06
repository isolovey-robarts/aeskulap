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
*/

#ifndef IMAGEPOOL_MOVEASSOCIATION_H
#define IMAGEPOOL_MOVEASSOCIATION_H

#include "poolfindassociation.h"

class MoveAssociation : public FindAssociation {
public:

	MoveAssociation();
	virtual ~MoveAssociation();

	void Create(const std::string& title, const std::string& peer, int port, const std::string& ouraet, /*int ourPort,*/ const char *abstractSyntax = UID_MOVEPatientRootQueryRetrieveInformationModel/*, const char *abstractSyntaxMove = UID_MOVEPatientRootQueryRetrieveInformationModel*/);

	OFCondition SendObject(DcmDataset *dataset);

protected:

	void OnAddPresentationContext(T_ASC_Parameters *params, const char* transferSyntaxList[], int transferSyntaxListCount);

	virtual void OnResponseReceived(DcmDataset* response);

	int m_maxReceivePDULength;

private:

	typedef struct _MoveCallbackInfo {
		T_ASC_Association *assoc;
		T_ASC_PresentationContextID presId;
		MoveAssociation* pCaller;
	} MoveCallbackInfo;

	typedef struct _StoreCallbackInfo {
		DcmDataset* dataset;
		MoveAssociation* pCaller;
	} StoreCallbackInfo;

	OFCondition moveSCU(DcmDataset *pdset);
	static void moveCallback(void *callbackData, T_DIMSE_C_MoveRQ *request, int responseCount, T_DIMSE_C_MoveRSP *response);

	OFCondition acceptSubAssoc(T_ASC_Network *aNet, T_ASC_Association **assoc);
	OFCondition subOpSCP(T_ASC_Association **subAssoc);
	static void subOpCallback(void * pCaller, T_ASC_Network *aNet, T_ASC_Association **subAssoc);

	OFCondition storeSCP(T_ASC_Association *assoc, T_DIMSE_Message *msg, T_ASC_PresentationContextID presID);
	static void storeSCPCallback(void *callbackData, T_DIMSE_StoreProgress *progress, T_DIMSE_C_StoreRQ *req, char *imageFileName, DcmDataset **imageDataSet, T_DIMSE_C_StoreRSP *rsp, DcmDataset **statusDetail);

	OFCondition echoSCP(T_ASC_Association *assoc, T_DIMSE_Message *msg, T_ASC_PresentationContextID presID);
};

#endif // IMAGEPOOL_MOVEASSOCIATION_H
