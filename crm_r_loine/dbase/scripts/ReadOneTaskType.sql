SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].[ReadOneTaskType]
(@tt_code    udd_code)

AS
   SELECT 
      tt_code      = TaskType.tt_code,
      tt_name      = TaskType.tt_name,
      tt_actual    = TaskType.tt_actual,
      tt_mdate     = TaskType.tt_mdate,
      tt_muser     = Options.opt_name_first,
      tt_read_only = ISNULL(TrustedUser.tru_user - USER_ID(), 1)
   FROM  TaskType,
         TrustedUser,
         Options
   WHERE   TaskType.tt_code      = @tt_code
       AND TaskType.tt_deleted   = 0
       AND Options.opt_user          = TaskType.tt_muser
       AND TrustedUser.tru_owner     =* Options.opt_user
       AND TrustedUser.tru_user      = USER_ID()