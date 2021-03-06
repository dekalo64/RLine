SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].[ReadOnePartnerType]
(@part_code    udd_code)

AS
   SELECT 
      part_code      = PartnerType.part_code,
      part_name      = PartnerType.part_name,
      part_actual  	 = PartnerType.part_actual,
      part_mdate     = PartnerType.part_mdate,
      part_muser     = Options.opt_name_first,
      part_read_only = ISNULL(TrustedUser.tru_user - USER_ID(), 1)
   FROM  PartnerType,
         TrustedUser,
         Options
   WHERE   PartnerType.part_code    = @part_code
       AND PartnerType.part_deleted = 0
       AND Options.opt_user         = PartnerType.part_muser
       AND TrustedUser.tru_owner    =* Options.opt_user
       AND TrustedUser.tru_user     = USER_ID()
