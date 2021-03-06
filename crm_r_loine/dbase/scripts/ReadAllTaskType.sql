SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO


--
-- Definition for stored procedure ReadAllTaskType : 
--

--
-- Definition for stored procedure ReadAllTaskType : 
--

CREATE PROCEDURE [dbo].[ReadAllTaskType]
(@tt_actual    udd_type)

AS
   SELECT
      tt_code    = TaskType.tt_code,
      tt_name    = TaskType.tt_name,
      tt_actual  = TaskType.tt_actual
   FROM  TaskType
   WHERE TaskType.tt_actual >= @tt_actual
     AND TaskType.tt_deleted = 0
   ORDER BY
      TaskType.tt_name
