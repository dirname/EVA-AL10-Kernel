

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "ATCmdProc.h"
#include "AtCmdSupsProc.h"
#include "TafSsaApi.h"


/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/
/*lint -e767*/
#define    THIS_FILE_ID                 PS_FILE_ID_AT_CMD_SUPS_PROC_C
/*lint +e767*/

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
const AT_SS_EVT_FUNC_TBL_STRU           g_astAtSsEvtFuncTbl[] =
{
    /* 事件ID */                                /* 消息处理函数 */
    {ID_TAF_SSA_SET_LCS_MOLR_CNF,               AT_RcvSsaSetLcsMolrCnf},
    {ID_TAF_SSA_GET_LCS_MOLR_CNF,               AT_RcvSsaGetLcsMolrCnf},
    {ID_TAF_SSA_LCS_MOLR_NTF,                   AT_RcvSsaLcsMolrNtf},
    {ID_TAF_SSA_SET_LCS_MTLR_CNF,               AT_RcvSsaSetLcsMtlrCnf},
    {ID_TAF_SSA_GET_LCS_MTLR_CNF,               AT_RcvSsaGetLcsMtlrCnf},
    {ID_TAF_SSA_LCS_MTLR_NTF,                   AT_RcvSsaLcsMtlrNtf},
    {ID_TAF_SSA_SET_LCS_MTLRA_CNF,              AT_RcvSsaSetLcsMtlraCnf},
    {ID_TAF_SSA_GET_LCS_MTLRA_CNF,              AT_RcvSsaGetLcsMtlraCnf},
};


/*****************************************************************************
  3 函数实现
*****************************************************************************/
/*****************************************************************************
 函 数 名  : AT_ConvertTafSsaErrorCode
 功能描述  : AT模块转换SSA模块的错误码函数
 输入参数  : VOS_UINT8                           ucIndex
             TAF_ERROR_CODE_ENUM_UINT32          enErrorCode
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年10月20日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_ConvertTafSsaErrorCode(
    VOS_UINT8                           ucIndex,
    TAF_ERROR_CODE_ENUM_UINT32          enErrorCode)
{
    VOS_UINT32                          ulResult;

    if (TAF_ERR_NO_ERROR == enErrorCode)
    {
        return AT_OK;
    }

    ulResult    = AT_ERROR;

    if ( (enErrorCode >= TAF_ERR_LCS_BASE)
      && (enErrorCode <= TAF_ERR_LCS_UNKNOWN_ERROR) )
    {
        ulResult = enErrorCode - TAF_ERR_LCS_BASE + AT_CMOLRE_ERR_ENUM_BEGIN + 1;
        return ulResult;
    }

    ulResult = At_ChgTafErrorCode(ucIndex, enErrorCode);

    return ulResult;
}

/*****************************************************************************
 函 数 名  : AT_FillCmolrParaEnable
 功能描述  : [内部函数]填写+CMOLR命令参数<enable>
 输入参数  : TAF_SSA_LCS_MOLR_PARA_SET_STRU      *pstMolrPara;
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月11日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_FillCmolrParaEnable(
    TAF_SSA_LCS_MOLR_PARA_SET_STRU     *pstMolrPara
)
{
    /* <enable> */
    if ( (0 == gucAtParaIndex)
      || (0 == gastAtParaList[0].usParaLen) )
    {
        pstMolrPara->enEnable = TAF_SSA_LCS_MOLR_ENABLE_TYPE_DISABLE;
    }
    else
    {
        pstMolrPara->enEnable = (VOS_UINT8)gastAtParaList[0].ulParaValue;
    }

    return;
}

/*****************************************************************************
 函 数 名  : AT_FillCmolrParaMethod
 功能描述  : [内部函数]填写+CMOLR命令参数<method>
 输入参数  : TAF_SSA_LCS_MOLR_PARA_SET_STRU      *pstMolrPara;
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月11日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_FillCmolrParaMethod(
    TAF_SSA_LCS_MOLR_PARA_SET_STRU     *pstMolrPara
)
{
    if ( (1 >= gucAtParaIndex)
      || (0 == gastAtParaList[1].usParaLen) )
    {
        return;
    }

    /* <method> */
    pstMolrPara->bitOpMethod    = VOS_TRUE;
    pstMolrPara->enMethod       = (VOS_UINT8)gastAtParaList[1].ulParaValue;

    return;
}

/*****************************************************************************
 函 数 名  : AT_FillCmolrParaHorAcc
 功能描述  : [内部函数]填写+CMOLR命令参数<hor-acc-set>、<hor-acc>
 输入参数  : TAF_SSA_SET_LCS_MOLR_REQ_STRU      *pstMolrReq;
 输出参数  : 无
 返 回 值  : VOS_UINT32
             VOS_OK         -- 填写成功
             VOS_ERR        -- 参数错误
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月11日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_FillCmolrParaHorAcc(
    TAF_SSA_LCS_MOLR_PARA_SET_STRU     *pstMolrPara
)
{
    if ( (2 >= gucAtParaIndex)
      || (0 == gastAtParaList[2].usParaLen) )
    {
        return VOS_OK;
    }

    /* <hor-acc-set> */
    pstMolrPara->bitOpHorAccSet = VOS_TRUE;
    pstMolrPara->enHorAccSet    = (VOS_UINT8)gastAtParaList[2].ulParaValue;

    if (LCS_HOR_ACC_SET_PARAM == pstMolrPara->enHorAccSet)
    {
        /* <hor-acc> */
        /* 要求水平精度，但没有下发水平精度系数，返回失败 */
        if ( (3 >= gucAtParaIndex)
          || (0 == gastAtParaList[3].usParaLen) )
        {
            AT_WARN_LOG("AT_FillCmolrParaPartI: <hor-acc> is required!");
            return VOS_ERR;
        }

        pstMolrPara->ucHorAcc   = (VOS_UINT8)gastAtParaList[3].ulParaValue;
    }

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : AT_FillCmolrParaVerReq
 功能描述  : [内部函数]填写+CMOLR命令参数<ver-req>、<ver-acc-set>、<ver-acc>
 输入参数  : TAF_SSA_LCS_MOLR_PARA_SET_STRU      *pstMolrPara;
 输出参数  : 无
 返 回 值  : VOS_UINT32
             VOS_OK         -- 填写成功
             VOS_ERR        -- 参数错误
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月11日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_FillCmolrParaVerReq(
    TAF_SSA_LCS_MOLR_PARA_SET_STRU     *pstMolrPara
)
{
    if ( (4 >= gucAtParaIndex)
      || (0 == gastAtParaList[4].usParaLen) )
    {
        return VOS_OK;
    }

    /* <ver-req> */
    pstMolrPara->bitOpVerReq    = VOS_TRUE;
    pstMolrPara->enVerReq       = (VOS_UINT8)gastAtParaList[4].ulParaValue;

    if ( (LCS_VER_REQUESTED != pstMolrPara->enVerReq)
      || (5 >= gucAtParaIndex)
      || (0 == gastAtParaList[5].usParaLen) )
    {
        return VOS_OK;
    }

    /* <ver-acc-set> */
    pstMolrPara->bitOpVerAccSet = VOS_TRUE;
    pstMolrPara->enVerAccSet    = (VOS_UINT8)gastAtParaList[5].ulParaValue;

    if (LCS_VER_ACC_SET_PARAM == pstMolrPara->enVerAccSet)
    {
        /* <ver-acc> */
        /* 要求垂直精度，但没有下发垂直精度系数，返回失败 */
        if ( (6 >= gucAtParaIndex)
          || (0 == gastAtParaList[6].usParaLen) )
        {
            AT_WARN_LOG("AT_FillCmolrParaVerReq: <ver-acc> is required!");
            return VOS_ERR;
        }

        pstMolrPara->ucVerAcc   = (VOS_UINT8)gastAtParaList[6].ulParaValue;
    }

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : AT_FillCmolrParaVelReq
 功能描述  : [内部函数]填写+CMOLR命令参数<vel-req>
 输入参数  : TAF_SSA_LCS_MOLR_PARA_SET_STRU      *pstMolrPara;
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月11日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_FillCmolrParaVelReq(
    TAF_SSA_LCS_MOLR_PARA_SET_STRU     *pstMolrPara
)
{
    if ( (7 >= gucAtParaIndex)
      || (0 == gastAtParaList[7].usParaLen))
    {
        return;
    }

    /* <vel-req> */
    pstMolrPara->bitOpVelReq    = VOS_TRUE;
    pstMolrPara->enVelReq       = (VOS_UINT8)gastAtParaList[7].ulParaValue;

    return;
}

/*****************************************************************************
 函 数 名  : AT_FillCmolrParaRepMode
 功能描述  : [内部函数]填写+CMOLR命令参数<rep-mode>
 输入参数  : TAF_SSA_SET_LCS_MOLR_REQ_STRU      *pstMolrReq;
 输出参数  : 无
 返 回 值  : VOS_UINT32
             VOS_OK         -- 填写成功
             VOS_ERR        -- 参数错误
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月11日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_FillCmolrParaRepMode(
    TAF_SSA_LCS_MOLR_PARA_SET_STRU     *pstMolrPara
)
{
    if ( (8 >= gucAtParaIndex)
      || (0 == gastAtParaList[8].usParaLen) )
    {
        return VOS_OK;
    }

    /* <rep-mode> */
    pstMolrPara->bitOpRepMode   = VOS_TRUE;
    pstMolrPara->enRepMode      = (VOS_UINT8)gastAtParaList[8].ulParaValue;

    if (9 >= gucAtParaIndex)
    {
        return VOS_OK;
    }

    /* <timeout> */
    if (0 != gastAtParaList[9].usParaLen)
    {
        pstMolrPara->bitOpTimeout   = VOS_TRUE;
        pstMolrPara->usTimeOut      = (VOS_UINT16)gastAtParaList[9].ulParaValue;
    }

    /* <interval> */
    if (LCS_REP_MODE_PERIODIC_RPT == pstMolrPara->enRepMode)
    {
        if ( (10 < gucAtParaIndex)
          && (0 != gastAtParaList[10].usParaLen) )
        {
            pstMolrPara->bitOpInterval  = VOS_TRUE;
            pstMolrPara->usInterval     = (VOS_UINT16)gastAtParaList[10].ulParaValue;
        }
    }

    if ( (VOS_TRUE == pstMolrPara->bitOpTimeout)
      && (VOS_TRUE == pstMolrPara->bitOpInterval)
      && (pstMolrPara->usInterval < pstMolrPara->usTimeOut) )
    {
        AT_WARN_LOG("AT_FillCmolrParaPartI: <interval> is too short!");
        return VOS_ERR;
    }

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : AT_FillCmolrParaShapeRep
 功能描述  : [内部函数]填写+CMOLR命令参数<shape-rep>
 输入参数  : TAF_SSA_LCS_MOLR_PARA_SET_STRU      *pstMolrPara;
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月11日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_FillCmolrParaShapeRep(
    TAF_SSA_LCS_MOLR_PARA_SET_STRU     *pstMolrPara
)
{
    if ( (11 >= gucAtParaIndex)
      || (0 == gastAtParaList[11].usParaLen) )
    {
        return;
    }

    /* <shape-rep> */
    pstMolrPara->bitOpShapeRep  = VOS_TRUE;
    pstMolrPara->ucShapeRep     = (VOS_UINT8)gastAtParaList[11].ulParaValue;

    return;
}

/*****************************************************************************
 函 数 名  : AT_FillCmolrParaPlane
 功能描述  : [内部函数]填写+CMOLR命令参数<plane>
 输入参数  : TAF_SSA_LCS_MOLR_PARA_SET_STRU      *pstMolrPara;
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月11日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_FillCmolrParaPlane(
    TAF_SSA_LCS_MOLR_PARA_SET_STRU     *pstMolrPara
)
{
    if ( (12 >= gucAtParaIndex)
      || (0 == gastAtParaList[12].usParaLen) )
    {
        return;
    }

    /* <plane> */
    pstMolrPara->bitOpPlane     = VOS_TRUE;
    pstMolrPara->enPlane        = (VOS_UINT8)gastAtParaList[12].ulParaValue;

    return;
}

/*****************************************************************************
 函 数 名  : AT_Str2NmeaRep
 功能描述  : AT参数字符串类型转为NMEA格式类型
 输入参数  : VOS_UINT16                          usLength
             VOS_CHAR                           *pcStr
             TAF_SSA_LCS_NMEA_REP_STRU          *pstNmeaRep
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月12日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_Str2NmeaRep(
    VOS_UINT16                          usLength,
    VOS_CHAR                           *pcStr,
    TAF_SSA_LCS_NMEA_REP_STRU          *pstNmeaRep
)
{
    VOS_UINT32                          ulNum;
    VOS_UINT32                          ulLoop;
    VOS_CHAR                           *pcTemp  = VOS_NULL_PTR;
    VOS_CHAR                            aucSrcStr[AT_PARA_NMEA_MAX_LEN + 1];

    PS_MEM_SET(aucSrcStr, 0, sizeof(aucSrcStr));
    PS_MEM_SET(pstNmeaRep, 0, sizeof(TAF_SSA_LCS_NMEA_REP_STRU));

    if (usLength > AT_PARA_NMEA_MAX_LEN)
    {
        AT_WARN_LOG("AT_Str2NmeaRep: NMEA format string is too long!");
        return VOS_ERR;
    }

    ulNum = (usLength + 1) / (AT_PARA_NMEA_MIN_LEN + 1);

    if (0 == ulNum)
    {
        AT_WARN_LOG("AT_Str2NmeaRep: NMEA format string is too short!");
        return VOS_ERR;
    }

    PS_MEM_CPY(aucSrcStr, pcStr, usLength);
    pcTemp = aucSrcStr;

    for (ulLoop = 0; ulLoop < ulNum; ulLoop++)
    {
        /* $GPGGA */
        if (0 == VOS_MemCmp(pcTemp, AT_PARA_NMEA_GPGGA, AT_PARA_NMEA_MIN_LEN))
        {
            pstNmeaRep->bitGPGGA = VOS_TRUE;
        }
        /* $GPRMC */
        else if (0 == VOS_MemCmp(pcTemp, AT_PARA_NMEA_GPRMC, AT_PARA_NMEA_MIN_LEN))
        {
            pstNmeaRep->bitGPRMC = VOS_TRUE;
        }
        /* $GPGLL */
        else if (0 == VOS_MemCmp(pcTemp, AT_PARA_NMEA_GPGLL, AT_PARA_NMEA_MIN_LEN))
        {
            pstNmeaRep->bitGPGLL = VOS_TRUE;
        }
        else
        {
            AT_WARN_LOG("AT_Str2NmeaRep: NMEA format string is incorrect!");
            return VOS_ERR;
        }

        pcTemp += (AT_PARA_NMEA_MIN_LEN + 1);
    }

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : AT_FillCmolrParaNmeaRep
 功能描述  : [内部函数]填写+CMOLR命令<NMEA-rep>
 输入参数  : TAF_SSA_SET_LCS_MOLR_REQ_STRU      *pstMolrReq;
 输出参数  : 无
 返 回 值  : VOS_UINT32
             VOS_OK         -- 填写成功
             VOS_ERR        -- 参数错误
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月11日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_FillCmolrParaNmeaRep(
    TAF_SSA_LCS_MOLR_PARA_SET_STRU     *pstMolrPara
)
{
    if ( (13 >= gucAtParaIndex)
      || (0 == gastAtParaList[13].usParaLen) )
    {
        return VOS_OK;
    }

    /* <NMEA-rep> */
    if ( (TAF_SSA_LCS_MOLR_ENABLE_TYPE_NMEA     == pstMolrPara->enEnable)
      || (TAF_SSA_LCS_MOLR_ENABLE_TYPE_NMEA_GAD == pstMolrPara->enEnable) )
    {
        pstMolrPara->bitOpNmeaRep = VOS_TRUE;
        if (VOS_OK != AT_Str2NmeaRep(gastAtParaList[13].usParaLen,
                                     (VOS_CHAR *)gastAtParaList[13].aucPara,
                                     &(pstMolrPara->stNmeaRep)))
        {
            return VOS_ERR;
        }
    }

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : AT_FillCmolrParaThdPtyAddr
 功能描述  : [内部函数]填写+CMOLR命令参数<third-party-address>
 输入参数  : TAF_SSA_SET_LCS_MOLR_REQ_STRU      *pstMolrReq;
 输出参数  : 无
 返 回 值  : VOS_UINT32
             VOS_OK         -- 填写成功
             VOS_ERR        -- 参数错误
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月11日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_FillCmolrParaThdPtyAddr(
    TAF_SSA_LCS_MOLR_PARA_SET_STRU     *pstMolrPara
)
{
    if ( (14 >= gucAtParaIndex)
      || (0 == gastAtParaList[14].usParaLen) )
    {
        return VOS_OK;
    }

    /* <third-party-address> */
    if (TAF_SSA_LCS_THIRD_PARTY_ADDR_MAX_LEN < gastAtParaList[14].usParaLen)
    {
        AT_WARN_LOG("AT_FillCmolrParaPartIII: <third-party-address> is too long!");
        return VOS_ERR;;
    }

    pstMolrPara->bitOpThirdPartyAddr = VOS_TRUE;
    PS_MEM_CPY(pstMolrPara->acThirdPartyAddr,
               (VOS_CHAR *)gastAtParaList[14].aucPara,
               gastAtParaList[14].usParaLen);

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : AT_FillCmolrPara
 功能描述  : [内部函数]填写+CMOLR命令参数
 输入参数  : TAF_SSA_SET_LCS_MOLR_REQ_STRU      *pstMolrReq;
 输出参数  : 无
 返 回 值  : VOS_UINT32
             VOS_OK         -- 填写成功
             VOS_ERR        -- 参数错误
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月11日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_FillCmolrPara(
    TAF_SSA_LCS_MOLR_PARA_SET_STRU     *pstMolrPara
)
{
    PS_MEM_SET(pstMolrPara, 0, sizeof(TAF_SSA_LCS_MOLR_PARA_SET_STRU));

    /* +CMOLR=[<enable>[,<method>[,<hor-acc-set>[,<hor-acc>[,<ver-req>
               [,<ver-acc-set>[,<ver-acc>[,<vel-req> [,<rep-mode>[,<timeout>
               [,<interval>[,<shape-rep>[,<plane>[,<NMEA-rep>
               [,<third-party-address>]]]]]]]]]]]]]]]] */

    /* 参数过多，返回失败 */
    if (15 < gucAtParaIndex)
    {
        AT_WARN_LOG("AT_FillCmolrPara: too many parameters!");
        return VOS_ERR;
    }

    AT_FillCmolrParaEnable(pstMolrPara);

    AT_FillCmolrParaMethod(pstMolrPara);

    if (VOS_OK != AT_FillCmolrParaHorAcc(pstMolrPara))
    {
        return VOS_ERR;
    }

    if (VOS_OK != AT_FillCmolrParaVerReq(pstMolrPara))
    {
        return VOS_ERR;
    }

    AT_FillCmolrParaVelReq(pstMolrPara);

    if (VOS_OK != AT_FillCmolrParaRepMode(pstMolrPara))
    {
        return VOS_ERR;
    }

    AT_FillCmolrParaShapeRep(pstMolrPara);

    AT_FillCmolrParaPlane(pstMolrPara);

    if (VOS_OK != AT_FillCmolrParaNmeaRep(pstMolrPara))
    {
        return VOS_ERR;
    }

    if (VOS_OK != AT_FillCmolrParaThdPtyAddr(pstMolrPara))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : AT_SetCmolrPara
 功能描述  : +CMOLR设置命令处理函数
 输入参数  : VOS_UINT8 ucIndex
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月9日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_SetCmolrPara(VOS_UINT8 ucIndex)
{
    TAF_SSA_LCS_MOLR_PARA_SET_STRU      stMolrPara;

    PS_MEM_SET(&stMolrPara, 0, sizeof(stMolrPara));

    /* 参数检查 */
    if (AT_CMD_OPT_SET_PARA_CMD != g_stATParseCmd.ucCmdOptType)
    {
        AT_WARN_LOG("AT_SetCmolrPara: Option Type Incrrect!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (VOS_OK != AT_FillCmolrPara(&stMolrPara))
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 执行命令操作 */
    TAF_SSA_SetCmolrInfo(WUEPS_PID_AT,
                         gastAtClientTab[ucIndex].usClientId,
                         gastAtClientTab[ucIndex].opId,
                         &stMolrPara);

    /* 设置当前操作类型 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CMOLR_SET;

    /* 命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}

/*****************************************************************************
 函 数 名  : AT_RcvSsaSetLcsMolrCnf
 功能描述  : AT模块处理TAF层SSA模块回复的MOLR设置结果
 输入参数  : VOS_UINT8                           ucIndex
             VOS_VOID                           *pEvent
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年10月19日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_RcvSsaSetLcsMolrCnf(
    VOS_UINT8                           ucIndex,
    VOS_VOID                           *pEvent
)
{
    TAF_SSA_SET_LCS_MOLR_CNF_STRU      *pstSetLcsMolrCnf    = VOS_NULL_PTR;
    VOS_UINT32                          ulResult;

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvSsaSetLcsMolrCnf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    /* 判断当前操作类型是否为AT_CMD_CMOLR_SET */
    if (AT_CMD_CMOLR_SET != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvSsaSetLcsMolrCnf: WARNING:Not AT_CMD_CMOLR_SET!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    pstSetLcsMolrCnf    = (TAF_SSA_SET_LCS_MOLR_CNF_STRU *)pEvent;
    ulResult            = AT_ConvertTafSsaErrorCode(ucIndex, pstSetLcsMolrCnf->enResult);

    if (AT_OK != ulResult)
    {
        AT_WARN_LOG("AT_RcvSsaSetLcsMolrCnf: WARNING:Set ^CMOLR Failed!");
    }

    /* 输出结果 */
    gstAtSendData.usBufLen = 0;
    At_FormatResultData(ucIndex, ulResult);

    return;
}

/*****************************************************************************
 函 数 名  : AT_QryCmolrPara
 功能描述  : +CMOLR查询命令处理函数
 输入参数  : VOS_UINT8 ucIndex
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月9日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_QryCmolrPara(VOS_UINT8 ucIndex)
{
    /* 执行查询操作 */
    TAF_SSA_GetCmolrInfo(WUEPS_PID_AT,
                         gastAtClientTab[ucIndex].usClientId,
                         gastAtClientTab[ucIndex].opId);

    /* 设置当前操作类型 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CMOLR_READ;

    /* 命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}

/*****************************************************************************
 函 数 名  : AT_RcvSsaGetLcsMolrCnf
 功能描述  : AT模块处理TAF层SSA模块回复的MOLR查询结果
 输入参数  : VOS_UINT8                           ucIndex
             VOS_VOID                           *pEvent
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年10月19日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_RcvSsaGetLcsMolrCnf(
    VOS_UINT8                           ucIndex,
    VOS_VOID                           *pEvent
)
{
    TAF_SSA_GET_LCS_MOLR_CNF_STRU      *pstGetLcsMolrCnf    = VOS_NULL_PTR;
    VOS_UINT32                          ulResult;
    VOS_BOOL                            bNmeaFlg;
    VOS_UINT16                          usLength;
    VOS_UINT8                           aucTPAStr[LCS_CLIENT_EXID_MAX_LEN + 1];

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvSsaGetLcsMolrCnf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    /* 判断当前操作类型是否为AT_CMD_CMOLR_READ */
    if (AT_CMD_CMOLR_READ != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvSsaGetLcsMolrCnf: WARNING:Not AT_CMD_CMOLR_READ!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    pstGetLcsMolrCnf    = (TAF_SSA_GET_LCS_MOLR_CNF_STRU *)pEvent;
    ulResult            = AT_ConvertTafSsaErrorCode(ucIndex, pstGetLcsMolrCnf->enResult);
    bNmeaFlg            = VOS_FALSE;
    usLength            = 0;

    /* 判断查询操作是否成功 */
    if (AT_OK == ulResult)
    {
        usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                          (VOS_CHAR *)pgucAtSndCodeAddr,
                                          (VOS_CHAR *)pgucAtSndCodeAddr,
                                          "%s: %d,%d,%d,",
                                          g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                          pstGetLcsMolrCnf->enEnable,
                                          pstGetLcsMolrCnf->stMolrPara.enMethod,
                                          pstGetLcsMolrCnf->stMolrPara.enHorAccSet);

        if (LCS_HOR_ACC_SET_PARAM == pstGetLcsMolrCnf->stMolrPara.enHorAccSet)
        {
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%d",
                                               pstGetLcsMolrCnf->stMolrPara.ucHorAcc);
        }

        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                           (VOS_CHAR *)pgucAtSndCodeAddr,
                                           (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                           ",%d,",
                                           pstGetLcsMolrCnf->stMolrPara.enVerReq);

        if (LCS_VER_REQUESTED == pstGetLcsMolrCnf->stMolrPara.enVerReq)
        {
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%d,",
                                               pstGetLcsMolrCnf->stMolrPara.enVerAccSet);

            if (LCS_VER_ACC_SET_PARAM == pstGetLcsMolrCnf->stMolrPara.enVerAccSet)
            {
                usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                                   "%d",
                                                   pstGetLcsMolrCnf->stMolrPara.ucVerAcc);
            }
        }
        else
        {
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               ",");
        }

        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                           (VOS_CHAR *)pgucAtSndCodeAddr,
                                           (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                           ",%d,%d,%d,",
                                           pstGetLcsMolrCnf->stMolrPara.enVelReq,
                                           pstGetLcsMolrCnf->stMolrPara.enRepMode,
                                           pstGetLcsMolrCnf->stMolrPara.usTimeOut);

        if (LCS_REP_MODE_PERIODIC_RPT == pstGetLcsMolrCnf->stMolrPara.enRepMode)
        {
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%d",
                                               pstGetLcsMolrCnf->stMolrPara.usInterval);
        }

        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                           (VOS_CHAR *)pgucAtSndCodeAddr,
                                           (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                           ",%d,%d,",
                                           pstGetLcsMolrCnf->stMolrPara.u.ucShapeRep,
                                           pstGetLcsMolrCnf->enPlane);

        if ( (TAF_SSA_LCS_MOLR_ENABLE_TYPE_NMEA == pstGetLcsMolrCnf->enEnable)
          || (TAF_SSA_LCS_MOLR_ENABLE_TYPE_NMEA_GAD == pstGetLcsMolrCnf->enEnable) )
        {
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "\"");

            if (VOS_TRUE == pstGetLcsMolrCnf->stNmeaRep.bitGPGGA)
            {
                bNmeaFlg  = VOS_TRUE;
                usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                                   "%s,",
                                                   AT_PARA_NMEA_GPGGA);
            }

            if (VOS_TRUE == pstGetLcsMolrCnf->stNmeaRep.bitGPRMC)
            {
                bNmeaFlg  = VOS_TRUE;
                usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                                   "%s,",
                                                   AT_PARA_NMEA_GPRMC);
            }

            if (VOS_TRUE == pstGetLcsMolrCnf->stNmeaRep.bitGPGLL)
            {
                bNmeaFlg  = VOS_TRUE;
                usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                                   "%s,",
                                                   AT_PARA_NMEA_GPGLL);
            }
            /* 删除多打印的一个字符 */
            usLength -= 1;

            if (VOS_TRUE == bNmeaFlg)
            {
                usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                                   "\"");
            }
        }

        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                           (VOS_CHAR *)pgucAtSndCodeAddr,
                                           (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                           ",");

        if ( ((LCS_MOLR_METHOD_TRANSFER_TP_ADDR == pstGetLcsMolrCnf->stMolrPara.enMethod)
           || (LCS_MOLR_METHOD_RETRIEVAL_TP_ADDR == pstGetLcsMolrCnf->stMolrPara.enMethod))
          && ((pstGetLcsMolrCnf->stMolrPara.stTPAddr.ucLength > 0)
           && (pstGetLcsMolrCnf->stMolrPara.stTPAddr.ucLength <= LCS_CLIENT_EXID_MAX_LEN)) )
        {
            PS_MEM_SET(aucTPAStr, 0, LCS_CLIENT_EXID_MAX_LEN + 1);
            PS_MEM_CPY(aucTPAStr,
                       pstGetLcsMolrCnf->stMolrPara.stTPAddr.aucValue,
                       pstGetLcsMolrCnf->stMolrPara.stTPAddr.ucLength);

            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "\"%s\"",
                                               aucTPAStr);
        }

    }

    /* 输出结果 */
    gstAtSendData.usBufLen = usLength;
    At_FormatResultData(ucIndex, ulResult);

    return;
}

/*****************************************************************************
 函 数 名  : AT_TestCmolrPara
 功能描述  : +CMOLR测试命令处理函数
 输入参数  : VOS_UINT8 ucIndex
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月9日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_TestCmolrPara(VOS_UINT8 ucIndex)
{
    /* 输出测试命令结果 */
    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                (VOS_CHAR *)pgucAtSndCodeAddr,
                                (VOS_CHAR *)pgucAtSndCodeAddr,
                                "%s: (0-3),(0-6),(0,1),(0-127),(0,1),(0,1),(0-127),(0-4),"
                                "(0,1),(1-65535),(1-65535),(1-127),(0,1),"
                                /* "\"$GPGSA,$GPGGA,$GPGSV,$GPRMC,$GPVTG,$GPGLL\"," */
                                "\"$GPGGA,$GPRMC,$GPGLL\","
                                "\"<third-party-address>\"",     /* <third-party-address>参数暂不确定格式 */
                                g_stParseContext[ucIndex].pstCmdElement->pszCmdName);

    return AT_OK;
}

/*****************************************************************************
 函 数 名  : AT_RcvSsaLcsMolrNtf
 功能描述  : AT模块处理TAF层SSA模块上报的MOLR定位信息
 输入参数  : VOS_UINT8                           ucIndex
             VOS_VOID                           *pEvent
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年10月20日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_RcvSsaLcsMolrNtf(
    VOS_UINT8                           ucIndex,
    VOS_VOID                           *pEvent
)
{
    TAF_SSA_LCS_MOLR_NTF_STRU          *pstLcsMolrNtf   = VOS_NULL_PTR;
    AT_MODEM_AGPS_CTX_STRU             *pstAgpsCtx      = VOS_NULL_PTR;
    VOS_CHAR                           *pcLocationStr   = VOS_NULL_PTR;
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulReturnCodeIndex;
    VOS_UINT16                          usLength;

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvSsaLcsMolrNtf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    pstLcsMolrNtf       = (TAF_SSA_LCS_MOLR_NTF_STRU *)pEvent;
    pstAgpsCtx          = AT_GetModemAgpsCtxAddrFromClientId(gastAtClientTab[ucIndex].usClientId);
    ulResult            = AT_ConvertTafSsaErrorCode(ucIndex, pstLcsMolrNtf->enResult);
    ulReturnCodeIndex   = 0;
    usLength            = 0;

    /* 判断查询操作是否成功 */
    if (AT_OK == ulResult)
    {
        pcLocationStr = (VOS_CHAR*)PS_MEM_ALLOC(WUEPS_PID_AT, pstLcsMolrNtf->usLocationStrLen + 1);
        if (VOS_NULL_PTR == pcLocationStr)
        {
            AT_ERR_LOG("AT_RcvSsaLcsMolrNtf: Alloc Memory Failed!");
            return;
        }

        VOS_StrNCpy(pcLocationStr, pstLcsMolrNtf->acLocationStr, pstLcsMolrNtf->usLocationStrLen);
        pcLocationStr[pstLcsMolrNtf->usLocationStrLen] = '\0';

        if (TAF_SSA_LCS_MOLR_RPT_NMEA == pstLcsMolrNtf->enRptTypeChoice)
        {
            /* 输出+CMOLRN定位信息 */
            usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              "%s%s\"%s\"%s",
                                              gaucAtCrLf,
                                              gastAtStringTab[AT_STRING_CMOLRN].pucText,
                                              pcLocationStr,
                                              gaucAtCrLf);
        }
        else
        {
            /* 输出+CMOLRG定位信息 */
            usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              "%s%s%s%s",
                                              gaucAtCrLf,
                                              gastAtStringTab[AT_STRING_CMOLRG].pucText,
                                              pcLocationStr,
                                              gaucAtCrLf);
        }

        PS_MEM_FREE(WUEPS_PID_AT, pcLocationStr);
    }
    else
    {
        if (VOS_OK != AT_GetReturnCodeId(ulResult, &ulReturnCodeIndex))
        {
            AT_ERR_LOG("AT_RcvSsaLcsMolrNtf: result code index is err!");
            return;
        }

        /* 输出+CMOLRE错误码 */
        if (AT_CMOLRE_NUMERIC == pstAgpsCtx->enCmolreType)
        {
            usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              "%s%s%s%s",
                                              gaucAtCrLf,
                                              gastAtStringTab[AT_STRING_CMOLRE].pucText,
                                              (VOS_CHAR *)gastAtReturnCodeTab[ulReturnCodeIndex].Result[0],
                                              gaucAtCrLf);
        }
        else
        {
            usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              "%s%s%s%s",
                                              gaucAtCrLf,
                                              gastAtStringTab[AT_STRING_CMOLRE].pucText,
                                              (VOS_CHAR *)gastAtReturnCodeTab[ulReturnCodeIndex].Result[1],
                                              gaucAtCrLf);
        }
    }

    /* 输出结果到AT通道 */
    At_SendResultData(ucIndex, pgucAtSndCodeAddr, usLength);

    return;
}

/*****************************************************************************
 函 数 名  : AT_SetCmolrePara
 功能描述  : +CMOLRE设置命令处理函数
 输入参数  : VOS_UINT8 ucIndex
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月9日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_SetCmolrePara(VOS_UINT8 ucIndex)
{
    AT_MODEM_AGPS_CTX_STRU             *pstAgpsCtx = VOS_NULL_PTR;

    pstAgpsCtx = AT_GetModemAgpsCtxAddrFromClientId(gastAtClientTab[ucIndex].usClientId);

    /* 参数检查 */
    if (AT_CMD_OPT_SET_PARA_CMD != g_stATParseCmd.ucCmdOptType)
    {
        AT_WARN_LOG("AT_SetCmolrePara: Option Type Incrrect!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多，返回失败 */
    if (1 < gucAtParaIndex)
    {
        AT_WARN_LOG("AT_SetCmolrePara: too many parameters!");
        return AT_TOO_MANY_PARA;
    }

    /* 执行命令操作 */
    if (0 == gastAtParaList[0].usParaLen)
    {
        pstAgpsCtx->enCmolreType = AT_CMOLRE_NUMERIC;
    }
    else
    {
        pstAgpsCtx->enCmolreType = (VOS_UINT8)gastAtParaList[0].ulParaValue;
    }

    /* 命令返回OK */
    return AT_OK;
}

/*****************************************************************************
 函 数 名  : AT_QryCmolrePara
 功能描述  : +CMOLRE查询命令处理函数
 输入参数  : VOS_UINT8 ucIndex
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月9日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_QryCmolrePara(VOS_UINT8 ucIndex)
{
    AT_MODEM_AGPS_CTX_STRU             *pstAgpsCtx = VOS_NULL_PTR;

    pstAgpsCtx = AT_GetModemAgpsCtxAddrFromClientId(gastAtClientTab[ucIndex].usClientId);

    /* 输出查询结果 */
    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                (VOS_CHAR *)pgucAtSndCodeAddr,
                                (VOS_CHAR *)pgucAtSndCodeAddr,
                                "%s: %d",
                                g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                pstAgpsCtx->enCmolreType);

    /* 命令返回OK */
    return AT_OK;
}

/*****************************************************************************
 函 数 名  : AT_SetCmtlrPara
 功能描述  : +CMTLR设置命令处理函数
 输入参数  : VOS_UINT8 ucIndex
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月9日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_SetCmtlrPara(VOS_UINT8 ucIndex)
{
    TAF_SSA_LCS_MTLR_SUBSCRIBE_ENUM_UINT8   enSubscribe;

    /* 参数检查 */
    if(AT_CMD_OPT_SET_PARA_CMD != g_stATParseCmd.ucCmdOptType)
    {
        AT_WARN_LOG("AT_SetCmtlrPara: Option Type Incrrect!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多，返回失败 */
    if (1 < gucAtParaIndex)
    {
        AT_WARN_LOG("AT_SetCmtlrPara: too many parameters!");
        return AT_TOO_MANY_PARA;
    }

    /* 执行命令操作 */
    if (0 == gastAtParaList[0].usParaLen)
    {
        enSubscribe = TAF_SSA_LCS_MOLR_ENABLE_TYPE_DISABLE;
    }
    else
    {
        enSubscribe = (VOS_UINT8)gastAtParaList[0].ulParaValue;
    }
    TAF_SSA_SetCmtlrInfo(WUEPS_PID_AT,
                         gastAtClientTab[ucIndex].usClientId,
                         gastAtClientTab[ucIndex].opId,
                         enSubscribe);

    /* 设置当前操作类型 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CMTLR_SET;

    /* 命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}

/*****************************************************************************
 函 数 名  : AT_RcvSsaSetLcsMtlrCnf
 功能描述  : AT模块处理TAF层SSA模块回复的MTLR设置结果
 输入参数  : VOS_UINT8                           ucIndex
             VOS_VOID                           *pEvent
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年10月20日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_RcvSsaSetLcsMtlrCnf(
    VOS_UINT8                           ucIndex,
    VOS_VOID                           *pEvent
)
{
    TAF_SSA_SET_LCS_MTLR_CNF_STRU      *pstSetLcsMtlrCnf    = VOS_NULL_PTR;
    VOS_UINT32                          ulResult;

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvSsaSetLcsMtlrCnf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    /* 判断当前操作类型是否为AT_CMD_CMTLR_SET */
    if (AT_CMD_CMTLR_SET != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvSsaSetLcsMtlrCnf: WARNING:Not AT_CMD_CMTLR_SET!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    pstSetLcsMtlrCnf    = (TAF_SSA_SET_LCS_MTLR_CNF_STRU *)pEvent;
    ulResult            = AT_ConvertTafSsaErrorCode(ucIndex, pstSetLcsMtlrCnf->enResult);

    if (AT_OK != ulResult)
    {
        AT_WARN_LOG("AT_RcvSsaSetLcsMtlrCnf: WARNING:Set ^CMTLR Failed!");
    }

    /* 输出结果 */
    gstAtSendData.usBufLen = 0;
    At_FormatResultData(ucIndex, ulResult);

    return;
}

/*****************************************************************************
 函 数 名  : AT_QryCmtlrPara
 功能描述  : +CMTLR查询命令处理函数
 输入参数  : VOS_UINT8 ucIndex
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月9日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_QryCmtlrPara(VOS_UINT8 ucIndex)
{
    /* 执行查询操作 */
    TAF_SSA_GetCmtlrInfo(WUEPS_PID_AT,
                         gastAtClientTab[ucIndex].usClientId,
                         gastAtClientTab[ucIndex].opId);

    /* 设置当前操作类型 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CMTLR_READ;

    /* 命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}

/*****************************************************************************
 函 数 名  : AT_RcvSsaGetLcsMtlrCnf
 功能描述  : AT模块处理TAF层SSA模块回复的MTLR查询结果
 输入参数  : VOS_UINT8                           ucIndex
             VOS_VOID                           *pEvent
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年10月19日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_RcvSsaGetLcsMtlrCnf(
    VOS_UINT8                           ucIndex,
    VOS_VOID                           *pEvent
)
{
    TAF_SSA_GET_LCS_MTLR_CNF_STRU      *pstGetLcsMtlrCnf    = VOS_NULL_PTR;
    VOS_UINT32                          ulResult;
    VOS_UINT16                          usLength;

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvSsaGetLcsMtlrCnf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    /* 判断当前操作类型是否为AT_CMD_CMTLR_READ */
    if (AT_CMD_CMTLR_READ != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvSsaGetLcsMtlrCnf: WARNING:Not AT_CMD_CMTLR_READ!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    pstGetLcsMtlrCnf    = (TAF_SSA_GET_LCS_MTLR_CNF_STRU *)pEvent;
    ulResult            = AT_ConvertTafSsaErrorCode(ucIndex, pstGetLcsMtlrCnf->enResult);
    usLength            = 0;

    /* 判断查询操作是否成功 */
    if (AT_OK == ulResult)
    {
        usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                          (VOS_CHAR *)pgucAtSndCodeAddr,
                                          (VOS_CHAR *)pgucAtSndCodeAddr,
                                          "%s: %d",
                                          g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                          pstGetLcsMtlrCnf->enSubscribe);
    }

    /* 输出结果 */
    gstAtSendData.usBufLen = usLength;
    At_FormatResultData(ucIndex, ulResult);

    return;
}

/*****************************************************************************
 函 数 名  : AT_RcvSsaLcsMtlrNtf
 功能描述  : AT模块处理TAF层SSA模块上报的MTLR网络定位请求
 输入参数  : VOS_UINT8                           ucIndex
             VOS_VOID                           *pEvent
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年10月20日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_RcvSsaLcsMtlrNtf(
    VOS_UINT8                           ucIndex,
    VOS_VOID                           *pEvent
)
{
    TAF_SSA_LCS_MTLR_NTF_STRU          *pstLcsMtlrNtf   = VOS_NULL_PTR;
    VOS_UINT32                          ulTmpStrLen;
    VOS_UINT32                          i;
    VOS_UINT16                          usLength;

    /* 只能为广播通道 */
    if (!AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvSsaLcsMtlrNtf: WARNING:Not AT_BROADCAST_INDEX!");
        return;
    }

    pstLcsMtlrNtf       = (TAF_SSA_LCS_MTLR_NTF_STRU *)pEvent;
    usLength            = 0;

    /* +CMTLR: <handle-id>,<notification-type>,<location-type>,
               [<client-external-id>],[<client-name>][,<plane>] */
    usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                      (VOS_CHAR *)pgucAtSndCodeAddr,
                                      (VOS_CHAR *)pgucAtSndCodeAddr,
                                      "%s%s%d,%d,%d,",
                                      gaucAtCrLf,
                                      gastAtStringTab[AT_STRING_CMTLR].pucText,
                                      pstLcsMtlrNtf->stMtlrPara.ucHandleId,
                                      pstLcsMtlrNtf->stMtlrPara.enNtfType,
                                      pstLcsMtlrNtf->stMtlrPara.enLocationType);

    if (VOS_TRUE == pstLcsMtlrNtf->stMtlrPara.bitOpClientExId)
    {
        ulTmpStrLen = pstLcsMtlrNtf->stMtlrPara.stClientExId.ucLength <= LCS_CLIENT_EXID_MAX_LEN ?
                        pstLcsMtlrNtf->stMtlrPara.stClientExId.ucLength : LCS_CLIENT_EXID_MAX_LEN;

        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                  (VOS_CHAR *)pgucAtSndCodeAddr,
                                  (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                  "\"");

        for (i = 0; i < ulTmpStrLen; i++)
        {
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                      (VOS_CHAR *)pgucAtSndCodeAddr,
                                      (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                      "%02X",
                                      pstLcsMtlrNtf->stMtlrPara.stClientExId.aucValue[i]);
        }

        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                  (VOS_CHAR *)pgucAtSndCodeAddr,
                                  (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                  "\"");

    }

    usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                      (VOS_CHAR *)pgucAtSndCodeAddr,
                                      (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                      ",");

    if (VOS_TRUE == pstLcsMtlrNtf->stMtlrPara.bitOpClientName)
    {
        ulTmpStrLen = pstLcsMtlrNtf->stMtlrPara.stClientName.ucLength <= LCS_CLIENT_NAME_MAX_LEN ?
                        pstLcsMtlrNtf->stMtlrPara.stClientName.ucLength : LCS_CLIENT_NAME_MAX_LEN;

        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                  (VOS_CHAR *)pgucAtSndCodeAddr,
                                  (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                  "\"");

        for (i = 0; i < ulTmpStrLen; i++)
        {
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                      (VOS_CHAR *)pgucAtSndCodeAddr,
                                      (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                      "%02X",
                                      pstLcsMtlrNtf->stMtlrPara.stClientName.aucValue[i]);
        }

        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                  (VOS_CHAR *)pgucAtSndCodeAddr,
                                  (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                  "\"");
    }

    if (VOS_TRUE == pstLcsMtlrNtf->stMtlrPara.bitOpPlane)
    {
        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                          (VOS_CHAR *)pgucAtSndCodeAddr,
                                          (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                          ",%d",
                                          pstLcsMtlrNtf->stMtlrPara.enPlane);
    }

    usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                      (VOS_CHAR *)pgucAtSndCodeAddr,
                                      (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                      "\r\n");

    /* 输出结果到AT通道 */
    At_SendResultData(ucIndex, pgucAtSndCodeAddr, usLength);

    return;
}

/*****************************************************************************
 函 数 名  : AT_SetCmtlraPara
 功能描述  : +CMTLRA设置命令处理函数
 输入参数  : VOS_UINT8 ucIndex
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月9日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_SetCmtlraPara(VOS_UINT8 ucIndex)
{
    TAF_SSA_LCS_MTLRA_PARA_SET_STRU     stCmtlraPara;

    /* 参数检查 */
    if(AT_CMD_OPT_SET_PARA_CMD != g_stATParseCmd.ucCmdOptType)
    {
        AT_WARN_LOG("AT_SetCmtlraPara: Option Type Incrrect!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数判断 */
    if (2 != gucAtParaIndex)
    {
        AT_WARN_LOG("AT_SetCmtlraPara: Incorrect Parameter Num!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 执行命令操作 */
    PS_MEM_SET(&stCmtlraPara, 0, sizeof(stCmtlraPara));
    stCmtlraPara.enAllow    = (VOS_UINT8)gastAtParaList[0].ulParaValue;
    stCmtlraPara.ucHandleId = (VOS_UINT8)gastAtParaList[1].ulParaValue;

    TAF_SSA_SetCmtlraInfo(WUEPS_PID_AT,
                          gastAtClientTab[ucIndex].usClientId,
                          gastAtClientTab[ucIndex].opId,
                          &stCmtlraPara);

    /* 设置当前操作类型 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CMTLRA_SET;

    /* 命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}

/*****************************************************************************
 函 数 名  : AT_RcvSsaSetLcsMtlraCnf
 功能描述  : AT模块处理TAF层SSA模块回复的MTLRA设置结果
 输入参数  : VOS_UINT8                           ucIndex
             VOS_VOID                           *pEvent
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年10月20日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_RcvSsaSetLcsMtlraCnf(
    VOS_UINT8                           ucIndex,
    VOS_VOID                           *pEvent
)
{
    TAF_SSA_SET_LCS_MTLRA_CNF_STRU     *pstSetLcsMtlraCnf    = VOS_NULL_PTR;
    VOS_UINT32                          ulResult;

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvSsaSetLcsMtlraCnf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    /* 判断当前操作类型是否为AT_CMD_CMTLRA_SET */
    if (AT_CMD_CMTLRA_SET != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvSsaSetLcsMtlraCnf: WARNING:Not AT_CMD_CMTLRA_SET!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    pstSetLcsMtlraCnf   = (TAF_SSA_SET_LCS_MTLRA_CNF_STRU *)pEvent;
    ulResult            = AT_ConvertTafSsaErrorCode(ucIndex, pstSetLcsMtlraCnf->enResult);

    if (AT_OK != ulResult)
    {
        AT_WARN_LOG("AT_RcvSsaSetLcsMtlraCnf: WARNING:Set ^CMTLRA Failed!");
    }

    /* 输出结果 */
    gstAtSendData.usBufLen = 0;
    At_FormatResultData(ucIndex, ulResult);

    return;
}

/*****************************************************************************
 函 数 名  : AT_QryCmtlraPara
 功能描述  : +CMTLRA查询命令处理函数
 输入参数  : VOS_UINT8 ucIndex
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月9日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_QryCmtlraPara(VOS_UINT8 ucIndex)
{
    /* 执行查询操作 */
    TAF_SSA_GetCmtlraInfo(WUEPS_PID_AT,
                          gastAtClientTab[ucIndex].usClientId,
                          gastAtClientTab[ucIndex].opId);

    /* 设置当前操作类型 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CMTLRA_READ;

    /* 命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}

/*****************************************************************************
 函 数 名  : AT_RcvSsaGetLcsMtlraCnf
 功能描述  : AT模块处理TAF层SSA模块回复的MTLRA查询结果
 输入参数  : VOS_UINT8                           ucIndex
             VOS_VOID                           *pEvent
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年10月19日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_RcvSsaGetLcsMtlraCnf(
    VOS_UINT8                           ucIndex,
    VOS_VOID                           *pEvent
)
{
    TAF_SSA_GET_LCS_MTLRA_CNF_STRU     *pstGetLcsMtlraCnf    = VOS_NULL_PTR;
    VOS_UINT32                          ulResult;
    VOS_UINT16                          usLength;
    VOS_UINT8                           i;
    VOS_UINT8                           ucCnt;

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvSsaGetLcsMtlraCnf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    /* 判断当前操作类型是否为AT_CMD_CMTLRA_READ */
    if (AT_CMD_CMTLRA_READ != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvSsaGetLcsMtlraCnf: WARNING:Not AT_CMD_CMTLRA_READ!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    pstGetLcsMtlraCnf   = (TAF_SSA_GET_LCS_MTLRA_CNF_STRU *)pEvent;
    ulResult            = AT_ConvertTafSsaErrorCode(ucIndex, pstGetLcsMtlraCnf->enResult);
    usLength            = 0;

    /* 判断查询操作是否成功 */
    if (AT_OK == ulResult)
    {
        ucCnt = (pstGetLcsMtlraCnf->ucCnt > TAF_SSA_LCS_MTLR_MAX_NUM) ?
                    TAF_SSA_LCS_MTLR_MAX_NUM : pstGetLcsMtlraCnf->ucCnt;
        for (i = 0;  i < ucCnt; i++)
        {
            if (0 != i)
            {
                usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                                   (VOS_CHAR *)gaucAtCrLf);
            }

            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%s: %d,%d",
                                               g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                               pstGetLcsMtlraCnf->aenAllow[i],
                                               pstGetLcsMtlraCnf->aucHandleId[i]);

        }
    }

    /* 输出结果 */
    gstAtSendData.usBufLen = usLength;
    At_FormatResultData(ucIndex, ulResult);

    return;
}

/*****************************************************************************
 函 数 名  : AT_TestCmtlraPara
 功能描述  : +CMTLRA测试命令处理函数
 输入参数  : VOS_UINT8 ucIndex
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年9月9日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 AT_TestCmtlraPara(VOS_UINT8 ucIndex)
{
    /* 输出测试命令结果 */
    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                (VOS_CHAR *)pgucAtSndCodeAddr,
                                (VOS_CHAR *)pgucAtSndCodeAddr,
                                "%s: (0,1)",
                                g_stParseContext[ucIndex].pstCmdElement->pszCmdName);

    return AT_OK;
}

/*****************************************************************************
 函 数 名  : AT_RcvTafSsaEvt
 功能描述  : AT模块处理TAF层SSA模块上报的事件
 输入参数  : TAF_SSA_EVT_STRU                   *pstEvent
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年10月19日
    作    者   : l00198894
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID AT_RcvTafSsaEvt(
    TAF_SSA_EVT_STRU                   *pstEvent
)
{
    MN_AT_IND_EVT_STRU                 *pMsg        = VOS_NULL_PTR;
    TAF_CTRL_STRU                      *pstCtrl     = VOS_NULL_PTR;
    AT_SS_EVT_FUNC                      pEvtFunc    = VOS_NULL_PTR;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          i;

    /* 判断事件是否是历史SSA代码上报 */
    if (0 != pstEvent->ulEvtExt)
    {
        pMsg = (MN_AT_IND_EVT_STRU *)pstEvent;
        At_SsMsgProc(pMsg->aucContent, pMsg->usLen);
        return;
    }

    /* 初始化 */
    pstCtrl     = (TAF_CTRL_STRU*)(pstEvent->aucContent);

    if ( AT_FAILURE == At_ClientIdToUserId(pstCtrl->usClientId,
                                           &ucIndex) )
    {
        AT_WARN_LOG("AT_RcvTafSsaEvt: At_ClientIdToUserId FAILURE");
        return;
    }

    /* 在事件处理表中查找处理函数 */
    for (i = 0; i < AT_ARRAY_SIZE(g_astAtSsEvtFuncTbl); i++)
    {
        if ( pstEvent->enEvtId == g_astAtSsEvtFuncTbl[i].enEvtId )
        {
            /* 事件ID匹配 */
            pEvtFunc = g_astAtSsEvtFuncTbl[i].pEvtFunc;
            break;
        }
    }

    /* 如果处理函数存在则调用 */
    if ( VOS_NULL_PTR != pEvtFunc )
    {
        pEvtFunc(ucIndex, pstEvent->aucContent);
    }
    else
    {
        AT_ERR_LOG1("AT_RcvTafSsaEvt: Unexpected event received! <EvtId>", pstEvent->enEvtId);
    }

    return;
}






