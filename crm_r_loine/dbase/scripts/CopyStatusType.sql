USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[CopyStatusType]    Дата сценария: 04/26/2013 11:16:36 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure CopyStatusType : 
--

--
-- Definition for stored procedure CopyStatusType : 
--

CREATE PROCEDURE [dbo].[CopyStatusType]
(@st_code udd_code)

AS

DECLARE @st_name     udd_name, 
        @st_actual   udd_logical

/*      Read city data          */
SELECT
   @st_name    = StatusType.st_name + ' (K)',
   @st_actual  = StatusType.st_actual
FROM  StatusType
WHERE StatusType.st_code    = @st_code
  AND StatusType.st_deleted = 0

IF (@@rowcount > 0) BEGIN
   EXECUTE dbo.InsertStatusType  @st_name,
                               @st_actual
END
ELSE BEGIN
   SELECT
      st_code    = StatusType.st_code,
      st_name    = StatusType.st_name
   FROM  StatusType
   WHERE 1 = 2
END
