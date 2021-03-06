SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].[FindTaskType]
(@tt_actual    udd_type,
 @tt_name      udd_tinyname)

AS

SELECT
   tt_code   = TaskType.tt_code,
   tt_name   = TaskType.tt_name,
   tt_actual = TaskType.tt_actual
FROM  TaskType
WHERE TaskType.tt_name       =  @tt_name
  AND TaskType.tt_actual     >= @tt_actual
  AND TaskType.tt_deleted    = 0
  AND (PATINDEX(@tt_name, TaskType.tt_name)  != 0)