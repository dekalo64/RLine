SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].[FindPriorityType]
(@pt_actual    udd_type,
 @pt_name      udd_tinyname)

AS

SELECT
   pt_code   = PriorityType.pt_code,
   pt_name   = PriorityType.pt_name,
   pt_actual = PriorityType.pt_actual
FROM  PriorityType
WHERE PriorityType.pt_name       =  @pt_name
  AND PriorityType.pt_actual     >= @pt_actual
  AND PriorityType.pt_deleted    = 0
  AND (PATINDEX(@pt_name, PriorityType.pt_name)  != 0)
