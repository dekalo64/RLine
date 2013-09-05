SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].[CopyPriorityType]
(@pt_code udd_code)

AS

DECLARE @pt_name     udd_name, 
        @pt_actual   udd_logical,
        @pt_level    udd_code

SELECT
   @pt_name    = PriorityType.pt_name + ' (K)',
   @pt_actual  = PriorityType.pt_actual,
   @pt_level   = PriorityType.pt_level
FROM  PriorityType
WHERE PriorityType.pt_code    = @pt_code
  AND PriorityType.pt_deleted = 0

IF (@@rowcount > 0) BEGIN
   EXECUTE dbo.InsertPriorityType  @pt_name,
                                   @pt_actual,
                                   @pt_level
END
ELSE BEGIN
   SELECT
      pt_code    = PriorityType.pt_code,
      pt_name    = PriorityType.pt_name
   FROM  PriorityType
   WHERE 1 = 2
END