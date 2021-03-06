SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].[FindStatusType]
(@st_actual    udd_type,
 @st_name      udd_tinyname)

AS

SELECT
   st_code   = StatusType.st_code,
   st_name   = StatusType.st_name,
   st_actual = StatusType.st_actual
FROM  StatusType
WHERE StatusType.st_name     =  @st_name
  AND StatusType.st_actual   >= @st_actual
  AND StatusType.st_deleted  = 0
  AND (PATINDEX(@st_name, StatusType.st_name)  != 0)