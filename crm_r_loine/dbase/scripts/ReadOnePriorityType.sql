SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].[ReadOnePriorityType]
(@pt_code    udd_code)

AS
   SELECT 
      pt_code      = PriorityType.pt_code,
      pt_name      = PriorityType.pt_name,
      pt_actual    = PriorityType.pt_actual,
      pt_level     = PriorityType.pt_level,
      pt_mdate     = PriorityType.pt_mdate,
      pt_muser     = Options.opt_name_first,
      pt_read_only = ISNULL(TrustedUser.tru_user - USER_ID(), 1)
   FROM  PriorityType,
         TrustedUser,
         Options
   WHERE   PriorityType.pt_code      = @pt_code
       AND PriorityType.pt_deleted   = 0
       AND Options.opt_user          = PriorityType.pt_muser
       AND TrustedUser.tru_owner     =* Options.opt_user
       AND TrustedUser.tru_user      = USER_ID()