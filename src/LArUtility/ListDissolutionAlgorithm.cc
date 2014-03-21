/**
 *  @file   LArContent/src/LArUtility/ListDissolutionAlgorithm.cc
 * 
 *  @brief  Implementation of the list dissolution algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "LArUtility/ListDissolutionAlgorithm.h"

using namespace pandora;

namespace lar
{

StatusCode ListDissolutionAlgorithm::Run()
{
    for (StringVector::const_iterator iter = m_pfoListNames.begin(), iterEnd = m_pfoListNames.end(); iter != iterEnd; ++iter)
    {
        const std::string &listName(*iter);

        try
        {
            const PfoList *pPfoList = NULL;
            PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, != , PandoraContentApi::GetList(*this, listName, pPfoList));

            const PfoList pfoList(*pPfoList);

            for (PfoList::const_iterator pIter = pfoList.begin(), pIterEnd = pfoList.end(); pIter != pIterEnd; ++pIter)
           {
                if (STATUS_CODE_SUCCESS != PandoraContentApi::Delete(*this, *pIter, listName))
                {
                    std::cout << "ListDissolutionAlgorithm: Could not delete Pfo." << std::endl;
                }
            }
        }
        catch (StatusCodeException &)
        {
            std::cout << "ListDissolutionAlgorithm: Pfo list " << listName << " not found." << std::endl;
        }
    }

    for (StringVector::const_iterator iter = m_clusterListNames.begin(), iterEnd = m_clusterListNames.end(); iter != iterEnd; ++iter)
    {
        const std::string &listName(*iter);

        try
        {
            const ClusterList *pClusterList = NULL;
            PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, != , PandoraContentApi::GetList(*this, listName, pClusterList));

            const ClusterList clusterList(*pClusterList);

            for (ClusterList::const_iterator cIter = clusterList.begin(), cIterEnd = clusterList.end(); cIter != cIterEnd; ++cIter)
            {
                if (!m_warnIfClustersUnavailable && !(*cIter)->IsAvailable())
                    continue;

                if (STATUS_CODE_SUCCESS != PandoraContentApi::Delete(*this, *cIter, listName))
                {
                    if (m_warnIfClustersUnavailable)
                        std::cout << "ListDissolutionAlgorithm: Could not delete Pfo." << std::endl;
                }
            }
        }
        catch (StatusCodeException &)
        {
            std::cout << "ListDissolutionAlgorithm: Cluster list " << listName << " not found." << std::endl;
        }
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode ListDissolutionAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    m_pfoListNames.clear();
    m_clusterListNames.clear();

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadVectorOfValues(xmlHandle,
        "PfoListNames", m_pfoListNames));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadVectorOfValues(xmlHandle,
        "ClusterListNames", m_clusterListNames));

    m_warnIfClustersUnavailable = true;
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "WarnIfClustersUnavailable", m_warnIfClustersUnavailable));

    return STATUS_CODE_SUCCESS;
}

} // namespace lar
