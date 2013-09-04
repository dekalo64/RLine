USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[DeleteTaskType]    Дата сценария: 04/25/2013 22:56:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure DeleteTaskType : 
--

--
-- Definition for stored procedure DeleteTaskType : 
--

CREATE PROCEDURE [dbo].[DeleteTaskType]
(@tt_code      udd_code)

AS

UPDATE TaskType
SET   TaskType.tt_deleted = 1,
      TaskType.tt_mdate   = GETDATE()
FROM  TrustedUser
WHERE TaskType.tt_code         = @tt_code
  AND TrustedUser.tru_owner = TaskType.tt_muser
  AND TrustedUser.tru_user  = USER_ID()
/*      Return result           */
SELECT deleted = @@rowcount
